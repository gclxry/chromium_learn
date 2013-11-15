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

// An abstraction that keeps track of whether the caller is started or stopped
// and only allows the following transitions NOT_STARTED -> STARTED ->
// STOPPED. This class is thread-safe.

#ifndef GOOGLE_CACHEINVALIDATION_IMPL_RUN_STATE_H_
#define GOOGLE_CACHEINVALIDATION_IMPL_RUN_STATE_H_

#include "google/cacheinvalidation/deps/logging.h"
#include "google/cacheinvalidation/deps/mutex.h"

namespace invalidation {

class RunState {
 public:
  RunState() : current_state_(NOT_STARTED) {}

  /* Marks the current state to be STARTED.
   *
   * REQUIRES: Current state is NOT_STARTED.
   */
  void Start() {
    MutexLock m(&lock_);
    CHECK(current_state_ == NOT_STARTED) << "Cannot start: " << current_state_;
    current_state_ = STARTED;
  }

  /* Marks the current state to be STOPPED.
   *
   * REQUIRES: Current state is STARTED.
   */
  void Stop() {
    MutexLock m(&lock_);
    CHECK(current_state_ == STARTED) << "Cannot stop: " << current_state_;
    current_state_ = STOPPED;
  }

  /* Returns true iff Start has been called on this but Stop has not been
   * called.
   */
  bool IsStarted() const {
    // Don't treat locking a mutex as mutation.
    MutexLock m((Mutex *) &lock_);  // NOLINT
    return current_state_ == STARTED;
  }

  /* Returns true iff Start and Stop have been called on this object. */
  bool IsStopped() const {
    // Don't treat locking a mutex as mutation.
    MutexLock m((Mutex *) &lock_);  // NOLINT
    return current_state_ == STOPPED;
  }

 private:
  /* Whether the instance has been started and/or stopped. */
  enum CurrentState {
    NOT_STARTED,
    STARTED,
    STOPPED
  };

  CurrentState current_state_;
  Mutex lock_;
};

}  // namespace invalidation

#endif  // GOOGLE_CACHEINVALIDATION_IMPL_RUN_STATE_H_
