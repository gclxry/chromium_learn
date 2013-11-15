// Copyright 2012 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Implementation of the Invalidation Client Library (Ticl).

#include "google/cacheinvalidation/impl/invalidation-client-impl.h"

#include <sstream>

#include "google/cacheinvalidation/client_test_internal.pb.h"
#include "google/cacheinvalidation/deps/callback.h"
#include "google/cacheinvalidation/deps/random.h"
#include "google/cacheinvalidation/deps/sha1-digest-function.h"
#include "google/cacheinvalidation/deps/string_util.h"
#include "google/cacheinvalidation/impl/exponential-backoff-delay-generator.h"
#include "google/cacheinvalidation/impl/invalidation-client-util.h"
#include "google/cacheinvalidation/impl/log-macro.h"
#include "google/cacheinvalidation/impl/persistence-utils.h"
#include "google/cacheinvalidation/impl/proto-converter.h"
#include "google/cacheinvalidation/impl/proto-helpers.h"
#include "google/cacheinvalidation/impl/recurring-task.h"
#include "google/cacheinvalidation/impl/smearer.h"

namespace invalidation {

using ::ipc::invalidation::RegistrationManagerStateP;

const char* InvalidationClientImpl::kClientTokenKey = "ClientToken";

// AcquireTokenTask

AcquireTokenTask::AcquireTokenTask(InvalidationClientImpl* client)
    : RecurringTask(
        "AcquireToken",
        client->internal_scheduler_,
        client->logger_,
        &client->smearer_,
        client->CreateExpBackOffGenerator(TimeDelta::FromMilliseconds(
            client->config_.network_timeout_delay_ms())),
        Scheduler::NoDelay(),
        TimeDelta::FromMilliseconds(
            client->config_.network_timeout_delay_ms())),
      client_(client) {
  }

bool AcquireTokenTask::RunTask() {
  // If token is still not assigned (as expected), sends a request.
  // Otherwise, ignore.
  if (client_->client_token_.empty()) {
    // Allocate a nonce and send a message requesting a new token.
    client_->set_nonce(IntToString(
        client_->internal_scheduler_->GetCurrentTime().ToInternalValue()));
    client_->protocol_handler_.SendInitializeMessage(
        client_->application_client_id_, client_->nonce_, "AcquireToken");
    // Reschedule to check state, retry if necessary after timeout.
    return true;
  } else {
    return false;  // Don't reschedule.
  }
}

// RegSyncHeartbeatTask

RegSyncHeartbeatTask::RegSyncHeartbeatTask(InvalidationClientImpl* client)
    : RecurringTask(
        "RegSyncHeartbeat",
        client->internal_scheduler_,
        client->logger_,
        &client->smearer_,
        client->CreateExpBackOffGenerator(TimeDelta::FromMilliseconds(
            client->config_.network_timeout_delay_ms())),
        TimeDelta::FromMilliseconds(
            client->config_.network_timeout_delay_ms()),
        TimeDelta::FromMilliseconds(
            client->config_.network_timeout_delay_ms())),
      client_(client) {
}

bool RegSyncHeartbeatTask::RunTask() {
  if (!client_->registration_manager_.IsStateInSyncWithServer()) {
    // Simply send an info message to ensure syncing happens.
    TLOG(client_->logger_, INFO, "Registration state not in sync with "
         "server: %s", client_->registration_manager_.ToString().c_str());
    client_->SendInfoMessageToServer(false, true /* request server summary */);
    return true;
  } else {
    TLOG(client_->logger_, INFO, "Not sending message since state is in sync");
    return false;
  }
}

// PersistentWriteTask

PersistentWriteTask::PersistentWriteTask(InvalidationClientImpl* client)
    : RecurringTask(
        "PersistentWrite",
        client->internal_scheduler_,
        client->logger_,
        &client->smearer_,
        client->CreateExpBackOffGenerator(TimeDelta::FromMilliseconds(
            client->config_.write_retry_delay_ms())),
        Scheduler::NoDelay(),
        TimeDelta::FromMilliseconds(
            client->config_.write_retry_delay_ms())),
      client_(client) {
}

bool PersistentWriteTask::RunTask() {
  if (client_->client_token_.empty() ||
      (client_->client_token_ == last_written_token_)) {
    // No work to be done
    return false;  // Do not reschedule
  }

  // Persistent write needs to happen.
  PersistentTiclState state;
  state.set_client_token(client_->client_token_);
  string serialized_state;
  PersistenceUtils::SerializeState(state, client_->digest_fn_.get(),
      &serialized_state);
  client_->storage_->WriteKey(InvalidationClientImpl::kClientTokenKey,
      serialized_state,
      NewPermanentCallback(this, &PersistentWriteTask::WriteCallback,
          client_->client_token_));
  return true;  // Reschedule after timeout to make sure that write does happen.
}

void PersistentWriteTask::WriteCallback(const string& token, Status status) {
  TLOG(client_->logger_, INFO, "Write state completed: %d, %s",
       status.IsSuccess(), status.message().c_str());
  if (status.IsSuccess()) {
    // Set lastWrittenToken to be the token that was written (NOT client_token_:
    // which could have changed while the write was happening).
    last_written_token_ = token;
  } else {
    client_->statistics_->RecordError(
        Statistics::ClientErrorType_PERSISTENT_WRITE_FAILURE);
  }
}

// HeartbeatTask

HeartbeatTask::HeartbeatTask(InvalidationClientImpl* client)
    : RecurringTask(
        "Heartbeat",
        client->internal_scheduler_,
        client->logger_,
        &client->smearer_,
        NULL,
        TimeDelta::FromMilliseconds(
            client->config_.heartbeat_interval_ms()),
        Scheduler::NoDelay()),
      client_(client) {
  next_performance_send_time_ = client_->internal_scheduler_->GetCurrentTime() +
      smearer()->GetSmearedDelay(TimeDelta::FromMilliseconds(
          client_->config_.perf_counter_delay_ms()));
}

bool HeartbeatTask::RunTask() {
  // Send info message. If needed, send performance counters and reset the next
  // performance counter send time.
  TLOG(client_->logger_, INFO, "Sending heartbeat to server: %s",
       client_->ToString().c_str());
  Scheduler *scheduler = client_->internal_scheduler_;
  bool must_send_perf_counters =
      next_performance_send_time_ > scheduler->GetCurrentTime();
  if (must_send_perf_counters) {
    next_performance_send_time_ = scheduler->GetCurrentTime() +
        client_->smearer_.GetSmearedDelay(TimeDelta::FromMilliseconds(
            client_->config_.perf_counter_delay_ms()));
  }

  TLOG(client_->logger_, INFO, "Sending heartbeat to server: %s",
       client_->ToString().c_str());
  client_->SendInfoMessageToServer(must_send_perf_counters,
      !client_->registration_manager_.IsStateInSyncWithServer());
  return true;  // Reschedule.
}

InvalidationClientImpl::InvalidationClientImpl(
    SystemResources* resources, Random* random, int client_type,
    const string& client_name, const ClientConfigP& config,
    const string& application_name, InvalidationListener* listener)
    : resources_(resources),
      internal_scheduler_(resources->internal_scheduler()),
      logger_(resources->logger()),
      storage_(new SafeStorage(resources->storage())),
      statistics_(new Statistics()),
      listener_(new CheckingInvalidationListener(
          listener, statistics_.get(), internal_scheduler_,
          resources_->listener_scheduler(), logger_)),
      config_(config),
      digest_fn_(new Sha1DigestFunction()),
      registration_manager_(logger_, statistics_.get(), digest_fn_.get()),
      msg_validator_(new TiclMessageValidator(logger_)),
      smearer_(random, config.smear_percent()),
      protocol_handler_(config.protocol_handler_config(), resources, &smearer_,
          statistics_.get(), application_name, this, msg_validator_.get()),
      random_(random) {
  storage_.get()->SetSystemResources(resources_);
  application_client_id_.set_client_name(client_name);
  application_client_id_.set_client_type(client_type);
  CreateSchedulingTasks();
  TLOG(logger_, INFO, "Created client: %s", ToString().c_str());
}

void InvalidationClientImpl::CreateSchedulingTasks() {
  acquire_token_task_.reset(new AcquireTokenTask(this));
  reg_sync_heartbeat_task_.reset(new RegSyncHeartbeatTask(this));
  persistent_write_task_.reset(new PersistentWriteTask(this));
  heartbeat_task_.reset(new HeartbeatTask(this));
}

void InvalidationClientImpl::InitConfig(ClientConfigP* config) {
  ProtoHelpers::InitConfigVersion(config->mutable_version());
  ProtocolHandler::InitConfig(config->mutable_protocol_handler_config());
}

void InvalidationClientImpl::InitConfigForTest(ClientConfigP* config) {
  ProtoHelpers::InitConfigVersion(config->mutable_version());
  config->set_network_timeout_delay_ms(2000);
  config->set_heartbeat_interval_ms(5000);
  config->set_write_retry_delay_ms(500);
  ProtocolHandler::InitConfigForTest(config->mutable_protocol_handler_config());
}

void InvalidationClientImpl::Start() {
  CHECK(!ticl_state_.IsStarted()) << "Already started";

  // Initialize the nonce so that we can maintain the invariant that exactly
  // one of "nonce" and "clientToken" is non-null.
  set_nonce(IntToString(
      internal_scheduler_->GetCurrentTime().ToInternalValue()));

  TLOG(logger_, INFO, "Starting with C++ config: %s",
       ProtoHelpers::ToString(config_).c_str());

  // Read the state blob and then schedule startInternal once the value is
  // there.
  ScheduleStartAfterReadingStateBlob();
}

void InvalidationClientImpl::StartInternal(const string& serialized_state) {
  CHECK(internal_scheduler_->IsRunningOnThread()) << "Not on internal thread";

  CHECK(resources_->IsStarted()) << "Resources must be started before starting "
      "the Ticl";

  // Initialize the session manager using the persisted client token.
  PersistentTiclState persistent_state;
  bool deserialized = false;
  if (!serialized_state.empty()) {
    deserialized = PersistenceUtils::DeserializeState(
        logger_, serialized_state, digest_fn_.get(), &persistent_state);
  }

  if (!serialized_state.empty() && !deserialized) {
    // In this case, we'll proceed as if we had no persistent state -- i.e.,
    // obtain a new client id from the server.
    statistics_->RecordError(
        Statistics::ClientErrorType_PERSISTENT_DESERIALIZATION_FAILURE);
    TLOG(logger_, SEVERE, "Failed deserializing persistent state: %s",
         ProtoHelpers::ToString(serialized_state).c_str());
  }
  if (deserialized) {
    // If we have persistent state, use the previously-stored token and send a
    // heartbeat to let the server know that we've restarted, since we may have
    // been marked offline.
    //
    // In the common case, the server will already have all of our
    // registrations, but we won't know for sure until we've gotten its summary.
    // We'll ask the application for all of its registrations, but to avoid
    // making the registrar redo the work of performing registrations that
    // probably already exist, we'll suppress sending them to the registrar.
    TLOG(logger_, INFO, "Restarting from persistent state: %s",
         ProtoHelpers::ToString(
             persistent_state.client_token()).c_str());
    set_nonce("");
    set_client_token(persistent_state.client_token());
    should_send_registrations_ = false;

    // Schedule an info message for the near future. We delay a little bit to
    // allow the application to reissue its registrations locally and avoid
    // triggering registration sync with the data center due to a hash mismatch.
    internal_scheduler_->Schedule(TimeDelta::FromMilliseconds(
        config_.initial_persistent_heartbeat_delay_ms()),
        NewPermanentCallback(this,
            &InvalidationClientImpl::SendInfoMessageToServer, false, true));

    // We need to ensure that heartbeats are sent, regardless of whether we
    // start fresh or from persistent state.  The line below ensures that they
    // are scheduled in the persistent startup case.  For the other case, the
    // task is scheduled when we acquire a token.
    heartbeat_task_.get()->EnsureScheduled("Startup-after-persistence");
  } else {
    // If we had no persistent state or couldn't deserialize the state that we
    // had, start fresh.  Request a new client identifier.
    //
    // The server can't possibly have our registrations, so whatever we get
    // from the application we should send to the registrar.
    TLOG(logger_, INFO, "Starting with no previous state");
    should_send_registrations_ = true;
    ScheduleAcquireToken("Startup");
  }
  // InvalidationListener.Ready() is called when the ticl has acquired a
  // new token.
}

void InvalidationClientImpl::Stop() {
  TLOG(logger_, WARNING, "Ticl being stopped: %s", ToString().c_str());
  if (ticl_state_.IsStarted()) {
    ticl_state_.Stop();
  }
}

void InvalidationClientImpl::Register(const ObjectId& object_id) {
  vector<ObjectId> object_ids;
  object_ids.push_back(object_id);
  PerformRegisterOperations(object_ids, RegistrationP_OpType_REGISTER);
}

void InvalidationClientImpl::Unregister(const ObjectId& object_id) {
  vector<ObjectId> object_ids;
  object_ids.push_back(object_id);
  PerformRegisterOperations(object_ids, RegistrationP_OpType_UNREGISTER);
}

void InvalidationClientImpl::PerformRegisterOperations(
    const vector<ObjectId>& object_ids, RegistrationP::OpType reg_op_type) {
  CHECK(!object_ids.empty()) << "Must specify some object id";

  CHECK(ticl_state_.IsStarted() || ticl_state_.IsStopped()) <<
      "Cannot call " << reg_op_type << " for object " <<
      " when the Ticl has not been started. If start has been " <<
      "called, caller must wait for InvalidationListener.Ready";
  if (ticl_state_.IsStopped()) {
    // The Ticl has been stopped. This might be some old registration op
    // coming in. Just ignore instead of crashing.
    TLOG(logger_, WARNING, "Ticl stopped: register (%d) of %d objects ignored.",
         reg_op_type, object_ids.size());
    return;
  }
  internal_scheduler_->Schedule(
      Scheduler::NoDelay(),
      NewPermanentCallback(
          this, &InvalidationClientImpl::PerformRegisterOperationsInternal,
          object_ids, reg_op_type));
}

void InvalidationClientImpl::PerformRegisterOperationsInternal(
    const vector<ObjectId>& object_ids, RegistrationP::OpType reg_op_type) {
  vector<ObjectIdP> object_id_protos;
  for (size_t i = 0; i < object_ids.size(); ++i) {
    const ObjectId& object_id = object_ids[i];
    ObjectIdP object_id_proto;
    ProtoConverter::ConvertToObjectIdProto(object_id, &object_id_proto);
    Statistics::IncomingOperationType op_type =
        (reg_op_type == RegistrationP_OpType_REGISTER) ?
        Statistics::IncomingOperationType_REGISTRATION :
        Statistics::IncomingOperationType_UNREGISTRATION;
    statistics_->RecordIncomingOperation(op_type);
    TLOG(logger_, INFO, "Register %s, %d",
         ProtoHelpers::ToString(object_id_proto).c_str(), reg_op_type);
    object_id_protos.push_back(object_id_proto);

    // Inform immediately of success so that the application is informed even if
    // the reply message from the server is lost. When we get a real ack from
    // the server, we do not need to inform the application.
    InvalidationListener::RegistrationState reg_state =
        ConvertOpTypeToRegState(reg_op_type);
    listener_->InformRegistrationStatus(this, object_id, reg_state);
  }


  // Update the registration manager state, then have the protocol client send a
  // message.
  vector<ObjectIdP> object_id_protos_to_send;
  registration_manager_.PerformOperations(object_id_protos, reg_op_type,
      &object_id_protos_to_send);

  // Check whether we should suppress sending registrations because we don't
  // yet know the server's summary.
  if (should_send_registrations_ && (!object_id_protos_to_send.empty())) {
    protocol_handler_.SendRegistrations(object_id_protos_to_send, reg_op_type);
  }
  reg_sync_heartbeat_task_.get()->EnsureScheduled("PerformRegister");
}

void InvalidationClientImpl::Acknowledge(const AckHandle& acknowledge_handle) {
  if (acknowledge_handle.IsNoOp()) {
    // Nothing to do. We do not increment statistics here since this is a no op
    // handle and statistics can only be acccessed on the scheduler thread.
    return;
  }
  internal_scheduler_->Schedule(
      Scheduler::NoDelay(),
      NewPermanentCallback(
          this, &InvalidationClientImpl::AcknowledgeInternal,
          acknowledge_handle));
}

void InvalidationClientImpl::AcknowledgeInternal(
    const AckHandle& acknowledge_handle) {
  // Validate the ack handle.

  // 1. Parse the ack handle first.
  AckHandleP ack_handle;
  ack_handle.ParseFromString(acknowledge_handle.handle_data());
  if (!ack_handle.IsInitialized()) {
    TLOG(logger_, WARNING, "Bad ack handle : %s",
         ProtoHelpers::ToString(acknowledge_handle.handle_data()).c_str());
    statistics_->RecordError(
        Statistics::ClientErrorType_ACKNOWLEDGE_HANDLE_FAILURE);
    return;
  }

  // 2. Validate ack handle - it should have a valid invalidation.
  if (!ack_handle.has_invalidation()
      || !msg_validator_->IsValid(ack_handle.invalidation())) {
    TLOG(logger_, WARNING, "Incorrect ack handle: %s",
         ProtoHelpers::ToString(ack_handle).c_str());
    statistics_->RecordError(
        Statistics::ClientErrorType_ACKNOWLEDGE_HANDLE_FAILURE);
    return;
  }

  // Currently, only invalidations have non-trivial ack handle.
  InvalidationP* invalidation = ack_handle.mutable_invalidation();
  invalidation->clear_payload();  // Don't send the payload back.
  statistics_->RecordIncomingOperation(
      Statistics::IncomingOperationType_ACKNOWLEDGE);
  protocol_handler_.SendInvalidationAck(*invalidation);
}

string InvalidationClientImpl::ToString() {
  return StringPrintf("Client: %s, %s",
                      ProtoHelpers::ToString(application_client_id_).c_str(),
                      ProtoHelpers::ToString(client_token_).c_str());
}

string InvalidationClientImpl::GetClientToken() {
  CHECK(client_token_.empty() || nonce_.empty());
  TLOG(logger_, FINE, "Return client token = %s",
       ProtoHelpers::ToString(client_token_).c_str());
  return client_token_;
}

void InvalidationClientImpl::HandleTokenChanged(
    const ServerMessageHeader& header, const string& new_token) {
  CHECK(internal_scheduler_->IsRunningOnThread()) << "Not on internal thread";

  // If the client token was valid, we have already checked in protocol
  // handler.  Otherwise, we need to check for the nonce, i.e., if we have a
  // nonce, the message must carry the same nonce.
  if (!nonce_.empty()) {
    if (header.token() == nonce_) {
      TLOG(logger_, INFO, "Accepting server message with matching nonce: %s",
           ProtoHelpers::ToString(nonce_).c_str());
      set_nonce("");
    } else {
      statistics_->RecordError(Statistics::ClientErrorType_NONCE_MISMATCH);
      TLOG(logger_, INFO,
           "Rejecting server message with mismatched nonce: Client = %s, "
           "Server = %s", ProtoHelpers::ToString(nonce_).c_str(),
           ProtoHelpers::ToString(header.token()).c_str());
      return;
    }
  }

  // The message is for us. Process it.
  HandleIncomingHeader(header);

  if (new_token.empty()) {
    TLOG(logger_, INFO, "Destroying existing token: %s",
         ProtoHelpers::ToString(client_token_).c_str());
    ScheduleAcquireToken("Destroy");
  } else {
    // We just received a new token. Start the regular heartbeats now.
    TLOG(logger_, INFO, "New token being assigned at client: %s, Old = %s",
         ProtoHelpers::ToString(new_token).c_str(),
         ProtoHelpers::ToString(client_token_).c_str());
    heartbeat_task_.get()->EnsureScheduled("Heartbeat-after-new-token");
    set_nonce("");
    set_client_token(new_token);
    persistent_write_task_.get()->EnsureScheduled("Write-after-new-token");
  }
}

void InvalidationClientImpl::ScheduleAcquireToken(const string& debug_string) {
  CHECK(internal_scheduler_->IsRunningOnThread()) << "Not on internal thread";
  set_client_token("");
  acquire_token_task_.get()->EnsureScheduled(debug_string);
}

void InvalidationClientImpl::HandleInvalidations(
    const ServerMessageHeader& header,
    const RepeatedPtrField<InvalidationP>& invalidations) {
  CHECK(internal_scheduler_->IsRunningOnThread()) << "Not on internal thread";
  HandleIncomingHeader(header);

  for (int i = 0; i < invalidations.size(); ++i) {
    const InvalidationP& invalidation = invalidations.Get(i);
    AckHandleP ack_handle_proto;
    ack_handle_proto.mutable_invalidation()->CopyFrom(invalidation);
    string serialized;
    ack_handle_proto.SerializeToString(&serialized);
    AckHandle ack_handle(serialized);
    if (ProtoConverter::IsAllObjectIdP(invalidation.object_id())) {
      TLOG(logger_, INFO, "Issuing invalidate all");
      listener_->InvalidateAll(this, ack_handle);
    } else {
      // Regular object. Could be unknown version or not.
      Invalidation inv;
      ProtoConverter::ConvertFromInvalidationProto(invalidation, &inv);
      TLOG(logger_, INFO, "Issuing invalidate: %s",
           ProtoHelpers::ToString(invalidation).c_str());
      if (invalidation.is_known_version()) {
        listener_->Invalidate(this, inv, ack_handle);
      } else {
        // Unknown version
        listener_->InvalidateUnknownVersion(this, inv.object_id(), ack_handle);
      }
    }
  }
}

void InvalidationClientImpl::HandleRegistrationStatus(
    const ServerMessageHeader& header,
    const RepeatedPtrField<RegistrationStatus>& reg_status_list) {
  CHECK(internal_scheduler_->IsRunningOnThread()) << "Not on internal thread";
  HandleIncomingHeader(header);

  vector<bool> local_processing_statuses;
  registration_manager_.HandleRegistrationStatus(
      reg_status_list, &local_processing_statuses);
  CHECK(local_processing_statuses.size() ==
        static_cast<size_t>(reg_status_list.size())) <<
      "Not all registration statuses were processed";

  // Inform app about the success or failure of each registration based
  // on what the registration manager has indicated.
  for (int i = 0; i < reg_status_list.size(); ++i) {
    const RegistrationStatus& reg_status = reg_status_list.Get(i);
    bool was_success = local_processing_statuses[i];
    TLOG(logger_, FINE, "Process reg status: %s",
         ProtoHelpers::ToString(reg_status).c_str());

    ObjectId object_id;
    ProtoConverter::ConvertFromObjectIdProto(
        reg_status.registration().object_id(), &object_id);
    // Only inform in the case of failure since the success path has already
    // been dealt with (the ticl issued informRegistrationStatus immediately
    // after receiving the register/unregister call).
    if (!was_success) {
      string description =
          (reg_status.status().code() == StatusP_Code_SUCCESS) ?
              "Registration discrepancy detected" :
              reg_status.status().description();

      // Note "success" shows up as transient failure in this scenario.
      bool is_permanent =
          (reg_status.status().code() == StatusP_Code_PERMANENT_FAILURE);
      listener_->InformRegistrationFailure(
          this, object_id, !is_permanent, description);
    }
  }
}

void InvalidationClientImpl::HandleRegistrationSyncRequest(
    const ServerMessageHeader& header) {
  CHECK(internal_scheduler_->IsRunningOnThread()) << "Not on internal thread";
  // Send all the registrations in the reg sync message.
  HandleIncomingHeader(header);

  // Generate a single subtree for all the registrations.
  RegistrationSubtree subtree;
  registration_manager_.GetRegistrations("", 0, &subtree);
  protocol_handler_.SendRegistrationSyncSubtree(subtree);
}

void InvalidationClientImpl::HandleInfoMessage(
    const ServerMessageHeader& header,
    const RepeatedField<InfoRequestMessage_InfoType>& info_types) {
  CHECK(internal_scheduler_->IsRunningOnThread()) << "Not on internal thread";
  HandleIncomingHeader(header);
  bool must_send_performance_counters = false;
  for (int i = 0; i < info_types.size(); ++i) {
    must_send_performance_counters =
        (info_types.Get(i) ==
         InfoRequestMessage_InfoType_GET_PERFORMANCE_COUNTERS);
    if (must_send_performance_counters) {
      break;
    }
  }
  SendInfoMessageToServer(must_send_performance_counters,
                          !registration_manager_.IsStateInSyncWithServer());
}

void InvalidationClientImpl::HandleErrorMessage(
      const ServerMessageHeader& header,
      ErrorMessage::Code code,
      const string& description) {
  CHECK(internal_scheduler_->IsRunningOnThread()) << "Not on internal thread";
  HandleIncomingHeader(header);

  // If it is an auth failure, we shut down the ticl.
  TLOG(logger_, SEVERE, "Received error message: %s, %s, %s",
         header.ToString().c_str(), ProtoHelpers::ToString(code).c_str(),
         description.c_str());

  // Translate the code to error reason.
  int reason;
  switch (code) {
    case ErrorMessage_Code_AUTH_FAILURE:
      reason = ErrorReason::AUTH_FAILURE;
      break;
    case ErrorMessage_Code_UNKNOWN_FAILURE:
      reason = ErrorReason::UNKNOWN_FAILURE;
      break;
    default:
      reason = ErrorReason::UNKNOWN_FAILURE;
      break;
  }
  // Issue an informError to the application.
  ErrorInfo error_info(reason, false, description, ErrorContext());
  listener_->InformError(this, error_info);

  // If this is an auth failure, remove registrations and stop the Ticl.
  // Otherwise do nothing.
  if (code != ErrorMessage_Code_AUTH_FAILURE) {
    return;
  }

  // If there are any registrations, remove them and issue registration
  // failure.
  vector<ObjectIdP> desired_registrations;
  registration_manager_.RemoveRegisteredObjects(&desired_registrations);
  TLOG(logger_, WARNING, "Issuing failure for %d objects",
       desired_registrations.size());
  for (size_t i = 0; i < desired_registrations.size(); ++i) {
    ObjectId object_id;
    ProtoConverter::ConvertFromObjectIdProto(
        desired_registrations[i], &object_id);
    listener_->InformRegistrationFailure(
        this, object_id, false, "Auth error");
  }

  // Schedule the stop on the listener work queue so that it happens after the
  // inform registration failure calls above
  resources_->listener_scheduler()->Schedule(
      Scheduler::NoDelay(),
      NewPermanentCallback(this, &InvalidationClientImpl::Stop));
}

void InvalidationClientImpl::GetRegistrationManagerStateAsSerializedProto(
    string* result) {
  RegistrationManagerStateP reg_state;
  registration_manager_.GetClientSummary(reg_state.mutable_client_summary());
  registration_manager_.GetServerSummary(reg_state.mutable_server_summary());
  vector<ObjectIdP> registered_objects;
  registration_manager_.GetRegisteredObjectsForTest(&registered_objects);
  for (size_t i = 0; i < registered_objects.size(); ++i) {
    reg_state.add_registered_objects()->CopyFrom(registered_objects[i]);
  }
  reg_state.SerializeToString(result);
}

void InvalidationClientImpl::GetStatisticsAsSerializedProto(
    string* result) {
  vector<pair<string, int> > properties;
  statistics_->GetNonZeroStatistics(&properties);
  InfoMessage info_message;
  for (size_t i = 0; i < properties.size(); ++i) {
    PropertyRecord* record = info_message.add_performance_counter();
    record->set_name(properties[i].first);
    record->set_value(properties[i].second);
  }
  info_message.SerializeToString(result);
}

void InvalidationClientImpl::HandleIncomingHeader(
    const ServerMessageHeader& header) {
  CHECK(internal_scheduler_->IsRunningOnThread()) << "Not on internal thread";
  CHECK(nonce_.empty()) <<
      "Cannot process server header " << header.ToString() <<
      " with non-empty nonce " << nonce_;

  if (header.registration_summary() != NULL) {
    // We've received a summary from the server, so if we were suppressing
    // registrations, we should now allow them to go to the registrar.
    should_send_registrations_ = true;
    registration_manager_.InformServerRegistrationSummary(
        *header.registration_summary());
  }
}

void InvalidationClientImpl::SendInfoMessageToServer(
    bool must_send_performance_counters, bool request_server_summary) {
  TLOG(logger_, INFO, "Sending info message to server");
  CHECK(internal_scheduler_->IsRunningOnThread()) << "Not on internal thread";

  // Make sure that you have the latest registration summary.
  vector<pair<string, int> > performance_counters;
  ClientConfigP* config_to_send = NULL;
  if (must_send_performance_counters) {
    statistics_->GetNonZeroStatistics(&performance_counters);
    config_to_send = &config_;
  }
  protocol_handler_.SendInfoMessage(performance_counters, config_to_send,
      request_server_summary);
}

void InvalidationClientImpl::set_nonce(const string& new_nonce) {
  CHECK(new_nonce.empty() || client_token_.empty()) <<
      "Tried to set nonce with existing token " << client_token_;
  nonce_ = new_nonce;
}

void InvalidationClientImpl::set_client_token(const string& new_client_token) {
  CHECK(new_client_token.empty() || nonce_.empty()) <<
      "Tried to set token with existing nonce " << nonce_;

  // If the ticl has not been started and we are getting a new token (either
  // from persistence or from the server, start the ticl and inform the
  // application.
  bool finish_starting_ticl = !ticl_state_.IsStarted() &&
      client_token_.empty() && !new_client_token.empty();
  client_token_ = new_client_token;

  if (finish_starting_ticl) {
    FinishStartingTiclAndInformListener();
  }
}

void InvalidationClientImpl::FinishStartingTiclAndInformListener() {
  CHECK(!ticl_state_.IsStarted());

  ticl_state_.Start();
  listener_->Ready(this);

  // We are not currently persisting our registration digest, so regardless of
  // whether or not we are restarting from persistent state, we need to query
  // the application for all of its registrations.
  listener_->ReissueRegistrations(this, RegistrationManager::kEmptyPrefix, 0);
  TLOG(logger_, INFO, "Ticl started: %s", ToString().c_str());
}

void InvalidationClientImpl::ScheduleStartAfterReadingStateBlob() {
  storage_->ReadKey(kClientTokenKey,
      NewPermanentCallback(this, &InvalidationClientImpl::ReadCallback));
}

void InvalidationClientImpl::ReadCallback(
    pair<Status, string> read_result) {
  string serialized_state;
  if (read_result.first.IsSuccess()) {
    serialized_state = read_result.second;
  } else {
    statistics_->RecordError(
        Statistics::ClientErrorType_PERSISTENT_READ_FAILURE);
    TLOG(logger_, WARNING, "Could not read state blob: %s",
         read_result.first.message().c_str());
  }
  // Call start now.
  internal_scheduler_->Schedule(
      Scheduler::NoDelay(),
      NewPermanentCallback(
          this, &InvalidationClientImpl::StartInternal, serialized_state));
}

ExponentialBackoffDelayGenerator*
InvalidationClientImpl::CreateExpBackOffGenerator(
    const TimeDelta& initial_delay) {
  return new ExponentialBackoffDelayGenerator(random_.get(), initial_delay,
      config_.max_exponential_backoff_factor());
}

InvalidationListener::RegistrationState
InvalidationClientImpl::ConvertOpTypeToRegState(RegistrationP::OpType
    reg_op_type) {
  InvalidationListener::RegistrationState reg_state =
      reg_op_type == RegistrationP_OpType_REGISTER ?
      InvalidationListener::REGISTERED :
      InvalidationListener::UNREGISTERED;
  return reg_state;
}

void InvalidationClientImpl::ChangeNetworkTimeoutDelayForTest(
    const TimeDelta& delay) {
  config_.set_network_timeout_delay_ms(delay.InMilliseconds());
  CreateSchedulingTasks();
}

void InvalidationClientImpl::ChangeHeartbeatDelayForTest(
    const TimeDelta& delay) {
  config_.set_heartbeat_interval_ms(delay.InMilliseconds());
  CreateSchedulingTasks();
}

}  // namespace invalidation
