// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/simple_browser_main.h"

#include <iostream>

#include "base/command_line.h"
#include "base/file_util.h"
#include "base/files/file_path.h"
#include "base/files/scoped_temp_dir.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/strings/sys_string_conversions.h"
#include "base/threading/thread_restrictions.h"
#include "base/utf_string_conversions.h"
#include "content/public/browser/browser_main_runner.h"
// #include "content/shell/shell.h"
#include "content/shell/shell_switches.h"
#include "net/base/net_util.h"
#include "webkit/support/webkit_support.h"


// Main routine for running as the Browser process.
int SimpleBrowserMain(const content::MainFunctionParams& parameters) {

  scoped_ptr<content::BrowserMainRunner> main_runner_(
      content::BrowserMainRunner::Create());

  int exit_code = main_runner_->Initialize(parameters);

  if (exit_code >= 0)
    return exit_code;

  if (CommandLine::ForCurrentProcess()->HasSwitch(
        switches::kCheckLayoutTestSysDeps)) {
    MessageLoop::current()->PostTask(FROM_HERE, MessageLoop::QuitClosure());
    main_runner_->Run();
    // content::Shell::CloseAllWindows();
    main_runner_->Shutdown();
    return 0;
  }

  exit_code = main_runner_->Run();
  main_runner_->Shutdown();

  return exit_code;
}
