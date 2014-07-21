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
// #include "content/shell/shell.h"
// #include "content/shell/shell_browser_context.h"
// #include "content/shell/shell_devtools_delegate.h"
#include "content/shell/shell_switches.h"
#include "googleurl/src/gurl.h"
#include "grit/net_resources.h"
#include "net/base/net_module.h"
#include "net/base/net_util.h"
#include "ui/base/resource/resource_bundle.h"


#if defined(USE_AURA) && defined(USE_X11)
#include "ui/base/touch/touch_factory_x11.h"
#endif

namespace content {

namespace {

static GURL GetStartupURL() {
  CommandLine* command_line = CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kContentBrowserTest))
    return GURL();
  const CommandLine::StringVector& args = command_line->GetArgs();

  if (args.empty())
    return GURL("http://www.google.com/");

  GURL url(args[0]);
  if (url.is_valid() && url.has_scheme())
    return url;

  return net::FilePathToFileURL(base::FilePath(args[0]));
}

base::StringPiece PlatformResourceProvider(int key) {
  if (key == IDR_DIR_HEADER_HTML) {
    base::StringPiece html_data =
        ui::ResourceBundle::GetSharedInstance().GetRawDataResource(
            IDR_DIR_HEADER_HTML);
    return html_data;
  }
  return base::StringPiece();
}

}  // namespace

SimpleBrowserMainParts::SimpleBrowserMainParts(
    const MainFunctionParams& parameters)
    : BrowserMainParts(),
      parameters_(parameters)/*,
      run_message_loop_(true),
      devtools_delegate_(NULL)*/ {
}

SimpleBrowserMainParts::~SimpleBrowserMainParts() {
}

#if !defined(OS_MACOSX)
void SimpleBrowserMainParts::PreMainMessageLoopStart() {
#if defined(USE_AURA) && defined(USE_X11)
  ui::TouchFactory::SetTouchDeviceListFromCommandLine();
#endif
}
#endif

void SimpleBrowserMainParts::PostMainMessageLoopStart() {
}

void SimpleBrowserMainParts::PreEarlyInitialization() {
}

void SimpleBrowserMainParts::PreMainMessageLoopRun() {
  //browser_context_.reset(new ShellBrowserContext(false));
  //off_the_record_browser_context_.reset(new ShellBrowserContext(true));

  //Shell::Initialize();
  //net::NetModule::SetResourceProvider(PlatformResourceProvider);

  //devtools_delegate_ = new ShellDevToolsDelegate(browser_context_.get());

  //if (!CommandLine::ForCurrentProcess()->HasSwitch(switches::kDumpRenderTree)) {
  //  Shell::CreateNewWindow(browser_context_.get(),
  //                         GetStartupURL(),
  //                         NULL,
  //                         MSG_ROUTING_NONE,
  //                         gfx::Size());
  //}

  //if (parameters_.ui_task) {
  //  parameters_.ui_task->Run();
  //  delete parameters_.ui_task;
  //  run_message_loop_ = false;
  //}
}

bool SimpleBrowserMainParts::MainMessageLoopRun(int* result_code)  {
  return !run_message_loop_;
}

void SimpleBrowserMainParts::PostMainMessageLoopRun() {
//#if defined(USE_AURA)
//  Shell::PlatformExit();
//#endif
//  if (devtools_delegate_)
//    devtools_delegate_->Stop();
//  browser_context_.reset();
//  off_the_record_browser_context_.reset();
}

}  // namespace
