// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/simple_content_browser_client.h"

#include "base/command_line.h"
#include "base/file_util.h"
#include "base/path_service.h"
#include "content/public/browser/notification_service.h"
#include "content/public/browser/notification_types.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/resource_dispatcher_host.h"
#include "content/public/browser/storage_partition.h"
#include "content/public/common/content_switches.h"
#include "content/simple/simple_browser_main_parts.h"
#include "content/simple/simple_browser_context.h"
#include "googleurl/src/gurl.h"
#include "webkit/glue/webpreferences.h"


namespace content {


SimpleContentBrowserClient::SimpleContentBrowserClient(){

}

SimpleContentBrowserClient::~SimpleContentBrowserClient() {
}

void SimpleContentBrowserClient::Observe(int type, const NotificationSource& source, const NotificationDetails& details) {
}

BrowserMainParts* SimpleContentBrowserClient::CreateBrowserMainParts(const MainFunctionParams& parameters) {
  simple_browser_main_parts_ = new SimpleBrowserMainParts(parameters);
  return simple_browser_main_parts_;

}

SimpleBrowserContext* SimpleContentBrowserClient::SimpleBrowserContextForBrowserContext(BrowserContext* content_browser_context) {
    if (content_browser_context == browser_context())
      return browser_context();
    DCHECK_EQ(content_browser_context, off_the_record_browser_context());
    return off_the_record_browser_context();
}

SimpleBrowserContext* SimpleContentBrowserClient::browser_context() {
  return simple_browser_main_parts_->browser_context();
}

SimpleBrowserContext* SimpleContentBrowserClient::off_the_record_browser_context() {
    return simple_browser_main_parts_->off_the_record_browser_context();
}

net::URLRequestContextGetter* SimpleContentBrowserClient::CreateRequestContext(
  BrowserContext* content_browser_context, ProtocolHandlerMap* protocol_handlers) {
    SimpleBrowserContext* simple_browser_context =
      SimpleBrowserContextForBrowserContext(content_browser_context);
    return simple_browser_context->CreateRequestContext(protocol_handlers);
}

net::URLRequestContextGetter* SimpleContentBrowserClient::CreateRequestContextForStoragePartition(
  BrowserContext* content_browser_context, const base::FilePath& partition_path,
  bool in_memory, ProtocolHandlerMap* protocol_handlers) {
    SimpleBrowserContext* simple_browser_context =
      SimpleBrowserContextForBrowserContext(content_browser_context);
    return simple_browser_context->CreateRequestContextForStoragePartition(
      partition_path, in_memory, protocol_handlers);
}

}  // namespace content
