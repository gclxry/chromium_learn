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
#include "content/simple/simple_browser_main.h"
#include "content/simple/simple_content_browser_client.h"
//#include "content/simple/simple_content_renderer_client.h"
#include "content/simple/simple_switches.h"
// #include "content/simple/webkit_test_platform_support.h"
#include "content/public/test/layouttest_support.h"
#include "net/cookies/cookie_monster.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/ui_base_paths.h"
#include "ui/base/ui_base_switches.h"
#include "ui/gl/gl_switches.h"

#include "ipc/ipc_message.h"  // For IPC_MESSAGE_LOG_ENABLED.

//#if defined(IPC_MESSAGE_LOG_ENABLED)
//#define IPC_MESSAGE_MACROS_LOG_ENABLED
//#include "content/public/common/content_ipc_logging.h"
//#define IPC_LOG_TABLE_ADD_ENTRY(msg_id, logger) \
//    content::RegisterIPCLogger(msg_id, logger)
//#include "content/simple/simple_messages.h"
//#endif



#include <initguid.h>
#include "base/logging_win.h"


namespace {

// If "Content Shell" doesn't show up in your list of trace providers in
// Sawbuck, add these registry entries to your machine (NOTE the optional
// Wow6432Node key for x64 machines):
// 1. Find:  HKLM\SOFTWARE\[Wow6432Node\]Google\Sawbuck\Providers
// 2. Add a subkey with the name "{6A3E50A4-7E15-4099-8413-EC94D8C2A4B6}"
// 3. Add these values:
//    "default_flags"=dword:00000001
//    "default_level"=dword:00000004
//    @="Content Shell"

// {6A3E50A4-7E15-4099-8413-EC94D8C2A4B6}
const GUID kContentShellProviderName = {
    0x6a3e50a4, 0x7e15, 0x4099,
        { 0x84, 0x13, 0xec, 0x94, 0xd8, 0xc2, 0xa4, 0xb6 } };

void InitLogging() {
  base::FilePath log_filename;
  PathService::Get(base::DIR_EXE, &log_filename);
  log_filename = log_filename.AppendASCII("content_shell.log");
  logging::InitLogging(
      log_filename.value().c_str(),
      logging::LOG_TO_BOTH_FILE_AND_SYSTEM_DEBUG_LOG,
      logging::LOCK_LOG_FILE,
      logging::DELETE_OLD_LOG_FILE,
      logging::DISABLE_DCHECK_FOR_NON_OFFICIAL_RELEASE_BUILDS);
  logging::SetLogItems(true, true, true, true);
}

}  // namespace

namespace content {

SimpleMainDelegate::SimpleMainDelegate() {
}

SimpleMainDelegate::~SimpleMainDelegate() {
}

bool SimpleMainDelegate::BasicStartupComplete(int* exit_code) {
  // Enable trace control and transport through event tracing for Windows.
  logging::LogEventProvider::Initialize(kContentShellProviderName);

  InitLogging();
  CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kDumpRenderTree)) {
    command_line.AppendSwitch(switches::kProcessPerTab);
    command_line.AppendSwitch(switches::kAllowFileAccessFromFiles);
    command_line.AppendSwitchASCII(
        switches::kUseGL, gfx::kGLImplementationOSMesaName);
    SetAllowOSMesaImageTransportForTesting();
    DisableSystemDragDrop();
    command_line.AppendSwitch(switches::kSkipGpuDataLoading);
    command_line.AppendSwitch(switches::kEnableExperimentalWebKitFeatures);
    command_line.AppendSwitch(switches::kEnableCssShaders);
    command_line.AppendSwitchASCII(switches::kTouchEvents,
                                   switches::kTouchEventsEnabled);
    if (command_line.HasSwitch(switches::kEnableSoftwareCompositing))
      command_line.AppendSwitch(switches::kEnableSoftwareCompositingGLAdapter);

    net::CookieMonster::EnableFileScheme();
    //if (!WebKitTestPlatformInitialize()) {
    //  if (exit_code)
    //    *exit_code = 1;
    //  return true;
    //}
  }
  SetContentClient(&content_client_);
  return false;
}

void SimpleMainDelegate::PreSandboxStartup() {
  InitializeResourceBundle();
}

int SimpleMainDelegate::RunProcess(
    const std::string& process_type,
    const MainFunctionParams& main_function_params) {
  if (!process_type.empty())
    return -1;

  return SimpleBrowserMain(main_function_params);
}

void SimpleMainDelegate::InitializeResourceBundle() {
  base::FilePath pak_file;
  base::FilePath pak_dir;
  PathService::Get(base::DIR_MODULE, &pak_dir);

  pak_file = pak_dir.Append(FILE_PATH_LITERAL("content_shell.pak"));
  ui::ResourceBundle::InitSharedInstanceWithPakPath(pak_file);
}

ContentBrowserClient* SimpleMainDelegate::CreateContentBrowserClient() {
  browser_client_.reset(new SimpleContentBrowserClient);
  return browser_client_.get();
}

//ContentRendererClient* SimpleMainDelegate::CreateContentRendererClient() {
//  renderer_client_.reset(new ShellContentRendererClient);
//  return renderer_client_.get();
//}

}  // namespace content
