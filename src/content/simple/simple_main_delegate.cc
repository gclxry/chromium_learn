// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/simple_main_delegate.h"

#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "content/public/browser/browser_main_runner.h"
#include "content/public/common/content_switches.h"
#include "content/public/common/url_constants.h"
#include "content/public/test/layouttest_support.h"
#include "content/simple/simple_browser_main.h"
#include "content/simple/simple_content_browser_client.h"
#include "net/cookies/cookie_monster.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/ui_base_paths.h"
#include "ui/base/ui_base_switches.h"
#include "ui/gl/gl_switches.h"

#include "ipc/ipc_message.h"  // For IPC_MESSAGE_LOG_ENABLED.


#include <initguid.h>
#include "base/logging_win.h"


namespace content {

SimpleMainDelegate::SimpleMainDelegate() {
}

SimpleMainDelegate::~SimpleMainDelegate() {
}

bool SimpleMainDelegate::BasicStartupComplete(int* exit_code) {

  // ¥”SimpleContentClient÷–…Ë÷√user agent
  SetContentClient(&content_client_);
  return false;
}

int SimpleMainDelegate::RunProcess(const std::string& process_type, const MainFunctionParams& main_function_params) {
    if (!process_type.empty())
      return -1;
    return SimpleBrowserMain(main_function_params);
}

ContentBrowserClient* SimpleMainDelegate::CreateContentBrowserClient() {
  browser_client_.reset(new SimpleContentBrowserClient);
  return browser_client_.get();
}

}  // namespace content
