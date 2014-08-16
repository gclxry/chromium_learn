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
#include "content/simple/simple_web_contents_view_delegate_creator.h"
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

WebContentsViewDelegate* SimpleContentBrowserClient::GetWebContentsViewDelegate(WebContents* web_contents) {
  return CreateSimpleWebContentsViewDelegate(web_contents);
}

}  // namespace content
