// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/simple_browser_main_parts.h"

#include "base/bind.h"
#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/message_loop.h"
#include "base/threading/thread.h"
#include "base/threading/thread_restrictions.h"
#include "cc/base/switches.h"
#include "content/public/common/content_switches.h"
#include "content/public/common/main_function_params.h"
#include "content/public/common/url_constants.h"
#include "content/simple/shell.h"
#include "content/simple/simple_browser_context.h"
#include "googleurl/src/gurl.h"
#include "grit/net_resources.h"
#include "net/base/net_module.h"
#include "net/base/net_util.h"
#include "ui/base/resource/resource_bundle.h"


namespace content {

SimpleBrowserMainParts::SimpleBrowserMainParts(const MainFunctionParams& parameters)
  : BrowserMainParts(), parameters_(parameters), run_message_loop_(true){
}

SimpleBrowserMainParts::~SimpleBrowserMainParts() {
}

void SimpleBrowserMainParts::PreMainMessageLoopRun() {
  browser_context_.reset(new SimpleBrowserContext(false));
  off_the_record_browser_context_.reset(new SimpleBrowserContext(true));

  Shell::Initialize();
  Shell::CreateNewWindow(browser_context_.get(), GURL("www.baidu.com"), NULL, MSG_ROUTING_NONE, gfx::Size());

  if (parameters_.ui_task) 
  {
    parameters_.ui_task->Run();
    delete parameters_.ui_task;
    run_message_loop_ = false;
  }
}

}  // namespace
