// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/simple_web_contents_delegate.h"

#include "base/auto_reset.h"
#include "base/command_line.h"
#include "base/message_loop.h"
#include "base/path_service.h"
#include "base/stringprintf.h"
#include "base/string_number_conversions.h"
#include "base/string_util.h"
#include "base/utf_string_conversions.h"
#include "content/public/browser/devtools_manager.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/notification_details.h"
#include "content/public/browser/notification_source.h"
#include "content/public/browser/notification_types.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_view.h"
#include "content/public/common/renderer_preferences.h"
#include "content/simple/simple_browser_main_parts.h"
#include "content/simple/simple_content_browser_client.h"

#include "stdafx.h"
#include "resource.h"
#include <windows.h>

#include "SimpleView.h"
#include "aboutdlg.h"
#include "MainFrm.h"


CAppModule _Module;

// Content area size for newly created windows.
static const int kTestWindowWidth = 1420;
static const int kTestWindowHeight = 750;

namespace content {

  int ThreadFunc(void* param)
  {
    SimpleWebContentsDelegate* shell = (SimpleWebContentsDelegate*)param;
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    CMainFrame wndMain;

    if(wndMain.CreateEx() == NULL)
    {
      ATLTRACE(_T("Main window creation failed!\n"));
      return 0;
    }

    SetParent(shell->web_contents_->GetView()->GetNativeView(), wndMain.m_clientview->m_hWnd);

    wndMain.ShowWindow(SW_SHOW);



    int nRet = theLoop.Run();

    _Module.RemoveMessageLoop();
    return nRet;
  }


  SimpleWebContentsDelegate::SimpleWebContentsDelegate(WebContents* web_contents){
      //registrar_.Add(this, NOTIFICATION_WEB_CONTENTS_TITLE_UPDATED,
      //  Source<WebContents>(web_contents));
      //windows_.push_back(this);

      //if (!shell_created_callback_.is_null()) {
      //  shell_created_callback_.Run(this);
      //  shell_created_callback_.Reset();
      //}
  }

  SimpleWebContentsDelegate::~SimpleWebContentsDelegate() {
  }

  int SimpleWebContentsDelegate::Run(LPTSTR lpstrCmdLine, int nCmdShow)
  {
    HANDLE hThread=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, this, 0, NULL);
    return TRUE;
  }

  void SimpleWebContentsDelegate::Initialize() {
    //_setmode(_fileno(stdout), _O_BINARY);
    //_setmode(_fileno(stderr), _O_BINARY);
    //INITCOMMONCONTROLSEX InitCtrlEx;
    //InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    //InitCtrlEx.dwICC  = ICC_STANDARD_CLASSES;
    //InitCommonControlsEx(&InitCtrlEx);
    //RegisterWindowClass();

    HRESULT hRes = ::CoInitialize(NULL);
    // If you are running on NT 4.0 or higher you can use the following call instead to 
    // make the EXE free threaded. This means that calls come in on a random RPC thread.
    //	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    ATLASSERT(SUCCEEDED(hRes));

    // this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
    ::DefWindowProc(NULL, 0, 0, 0L);

    AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

    hRes = _Module.Init(NULL, GetModuleHandle(NULL));
    ATLASSERT(SUCCEEDED(hRes));

    //int nRet = Run(_T(""), SW_SHOW);

    //_Module.Term();
    //::CoUninitialize();
  }

  SimpleWebContentsDelegate* SimpleWebContentsDelegate::CreateNewWindow(BrowserContext* browser_context,
    const GURL& url, SiteInstance* site_instance, int routing_id, const gfx::Size& initial_size) {
      WebContents::CreateParams create_params(browser_context, site_instance);
      create_params.routing_id = routing_id;
      if (!initial_size.IsEmpty())
        create_params.initial_size = initial_size;
      else
        create_params.initial_size = gfx::Size(kTestWindowWidth, kTestWindowHeight);
      WebContents* web_contents = WebContents::Create(create_params);
      SimpleWebContentsDelegate* shell = CreateShell(web_contents);
      if (!url.is_empty())
        shell->LoadURL(url);
      return shell;
  }

  SimpleWebContentsDelegate* SimpleWebContentsDelegate::CreateShell(WebContents* web_contents) {
    SimpleWebContentsDelegate* shell = new SimpleWebContentsDelegate(web_contents);
    //shell->PlatformCreateWindow(kTestWindowWidth, kTestWindowHeight);

    shell->web_contents_.reset(web_contents);
    web_contents->SetDelegate(shell);

    // shell->PlatformSetContents();
    

    //shell->PlatformResizeSubViews();
    shell->Run(_T(""), SW_SHOW);
    return shell;
  }

  void SimpleWebContentsDelegate::Observe(int type, const NotificationSource& source, const NotificationDetails& details) {
      //if (type == NOTIFICATION_WEB_CONTENTS_TITLE_UPDATED) 
      //{
      //  std::pair<NavigationEntry*, bool>* title =
      //    Details<std::pair<NavigationEntry*, bool> >(details).ptr();

      //  if (title->first) {
      //    string16 text = title->first->GetTitle();
      //    PlatformSetTitle(text);
      //  }
      //} else if (type == NOTIFICATION_WEB_CONTENTS_DESTROYED) 
      //{
      //  //devtools_frontend_ = NULL;
      //  registrar_.Remove(this, NOTIFICATION_WEB_CONTENTS_DESTROYED, source);
      //} else 
      //{
      //  NOTREACHED();
      //}
  }

  void SimpleWebContentsDelegate::LoadURL(const GURL& url) {
    NavigationController::LoadURLParams params(url);
    params.transition_type = PageTransitionFromInt(
      PAGE_TRANSITION_TYPED | PAGE_TRANSITION_FROM_ADDRESS_BAR);
    params.frame_name = std::string();
    web_contents_->GetController().LoadURLWithParams(params);
    web_contents_->GetView()->Focus();
  }


}  // namespace content
