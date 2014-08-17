// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/simple_url_request_context_getter.h"

#include "base/command_line.h"
#include "base/logging.h"
#include "base/string_number_conversions.h"
#include "base/string_util.h"
#include "base/strings/string_split.h"
#include "base/threading/worker_pool.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/common/content_switches.h"
#include "content/public/common/url_constants.h"
//#include "content/shell/shell_network_delegate.h"
//#include "content/shell/shell_switches.h"
#include "net/cert/cert_verifier.h"
#include "net/cookies/cookie_monster.h"
#include "net/dns/host_resolver.h"
#include "net/dns/mapped_host_resolver.h"
#include "net/ftp/ftp_network_layer.h"
#include "net/http/http_auth_handler_factory.h"
#include "net/http/http_cache.h"
#include "net/http/http_network_session.h"
#include "net/http/http_server_properties_impl.h"
#include "net/proxy/proxy_service.h"
#include "net/ssl/default_server_bound_cert_store.h"
#include "net/ssl/server_bound_cert_service.h"
#include "net/ssl/ssl_config_service_defaults.h"
#include "net/url_request/protocol_intercept_job_factory.h"
#include "net/url_request/static_http_user_agent_settings.h"
#include "net/url_request/url_request_context.h"
#include "net/url_request/url_request_context_storage.h"
#include "net/url_request/url_request_job_factory_impl.h"

namespace content {

namespace {

//void InstallProtocolHandlers(net::URLRequestJobFactoryImpl* job_factory,
//                             ProtocolHandlerMap* protocol_handlers) {
//  for (ProtocolHandlerMap::iterator it =
//           protocol_handlers->begin();
//       it != protocol_handlers->end();
//       ++it) {
//    bool set_protocol = job_factory->SetProtocolHandler(
//        it->first, it->second.release());
//    DCHECK(set_protocol);
//  }
//  protocol_handlers->clear();
//}

}  // namespace

SimpleURLRequestContextGetter::SimpleURLRequestContextGetter(
    bool ignore_certificate_errors,
    const base::FilePath& base_path,
    MessageLoop* io_loop,
    MessageLoop* file_loop,
    ProtocolHandlerMap* protocol_handlers) {
  // Must first be created on the UI thread.
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

}

SimpleURLRequestContextGetter::~SimpleURLRequestContextGetter() {
}

net::URLRequestContext* SimpleURLRequestContextGetter::GetURLRequestContext() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  url_request_context_.reset(new net::URLRequestContext());
  return url_request_context_.get();
}

scoped_refptr<base::SingleThreadTaskRunner> SimpleURLRequestContextGetter::GetNetworkTaskRunner() const {
    return BrowserThread::GetMessageLoopProxyForThread(BrowserThread::IO);
}

net::HostResolver* SimpleURLRequestContextGetter::host_resolver() {
  return url_request_context_->host_resolver();
}

}  // namespace content
