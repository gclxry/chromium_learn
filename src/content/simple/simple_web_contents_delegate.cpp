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

// Content area size for newly created windows.
static const int kTestWindowWidth = 1420;
static const int kTestWindowHeight = 750;

namespace content {

  SimpleWebContentsDelegate::SimpleWebContentsDelegate(){
      //windows_.push_back(this);

      //if (!shell_created_callback_.is_null()) {
      //  shell_created_callback_.Run(this);
      //  shell_created_callback_.Reset();
      //}
  }

  SimpleWebContentsDelegate::~SimpleWebContentsDelegate() {
  }

  void SimpleWebContentsDelegate::Initialize(BrowserContext* browser_context,
    const GURL& url, SiteInstance* site_instance, int routing_id, const gfx::Size& initial_size) {
      WebContents::CreateParams create_params(browser_context, site_instance);
      create_params.routing_id = routing_id;
      if (!initial_size.IsEmpty())
        create_params.initial_size = initial_size;
      else
        create_params.initial_size = gfx::Size(kTestWindowWidth, kTestWindowHeight);
      WebContents* web_contents = WebContents::Create(create_params);
      current_web_contents_ = web_contents;
      current_web_contents_->SetDelegate(this);
      SetParent(current_web_contents_->GetView()->GetNativeView(), window_);
      PostMessage(main_window_,WM_USER_CREATE_TAB, 0, (LPARAM)current_web_contents_);

      
      
      if (!url.is_empty())
      {
        LoadURL(url);
      }
  }

  void SimpleWebContentsDelegate::Observe(int type, const NotificationSource& source, const NotificationDetails& details) {
      if (type == NOTIFICATION_WEB_CONTENTS_TITLE_UPDATED) 
      {
         
        WebContents* wen_contents = (WebContents*)source.map_key();

        std::pair<NavigationEntry*, bool>* title =
          Details<std::pair<NavigationEntry*, bool> >(details).ptr();

        if (title->first) 
        {
          string16 text = title->first->GetTitle();

          for (std::vector<TAB_INFO>::iterator iter = tab_info_.begin(); iter != tab_info_.end(); ++iter)
          {
            if (wen_contents == iter->web_contents)
            {
              iter->title = text;
              PostMessage(main_window_,WM_USER_UPDATE_TAB, (WPARAM)iter->web_contents, (LPARAM)iter->hwnd);
              break;
            }
          }


          int ia = 0;
          ia++;
          //PlatformSetTitle(text);
        }
      } else if (type == NOTIFICATION_WEB_CONTENTS_DESTROYED) 
      {
        //devtools_frontend_ = NULL;
        registrar_.Remove(this, NOTIFICATION_WEB_CONTENTS_DESTROYED, source);
      } else 
      {
        NOTREACHED();
      }
  }

  void SimpleWebContentsDelegate::LoadURL(const GURL& url) {
    NavigationController::LoadURLParams params(url);
    params.transition_type = PageTransitionFromInt(
      PAGE_TRANSITION_TYPED | PAGE_TRANSITION_FROM_ADDRESS_BAR);
    params.frame_name = std::string();
    current_web_contents_->GetController().LoadURLWithParams(params);
    current_web_contents_->GetView()->Focus();
  }

  void SimpleWebContentsDelegate::WebContentsCreated(WebContents* source_contents,
    int64 source_frame_id,
    const string16& frame_name,
    const GURL& target_url,
    WebContents* new_contents) {

      WebContents* web_contents = new_contents;
      web_contents->SetDelegate(this);
      SetParent(web_contents->GetView()->GetNativeView(), window_);
      PostMessage(main_window_,WM_USER_CREATE_TAB, 0, (LPARAM)web_contents);
  }

  void SimpleWebContentsDelegate::SetHWND(HWND main_window, HWND client_window)
  {
    main_window_ = main_window;
    window_ = client_window;
  }

  void SimpleWebContentsDelegate::DidNavigateMainFramePostCommit(WebContents* web_contents) {
    //GURL url = current_web_contents_->GetURL();
    PostMessage(main_window_,WM_USER_SET_URL, 0, 0);
    int ia = 0;
    ia++;
  }

  void SimpleWebContentsDelegate::MakePair(HWND button_hwnd, LPARAM lParam)
  {
    WebContents* web_contents = (WebContents*)lParam;
    TAB_INFO ti;
    ti.hwnd = button_hwnd;
    ti.web_contents = web_contents;
    tab_info_.push_back(ti);

    current_web_contents_ = web_contents;

    registrar_.Add(this, NOTIFICATION_WEB_CONTENTS_TITLE_UPDATED,
      Source<WebContents>(web_contents));
  }

  void SimpleWebContentsDelegate::SwitchTab(HWND hwnd)
  {
    for (std::vector<TAB_INFO>::iterator iter = tab_info_.begin(); iter != tab_info_.end(); ++iter)
    {
      if (hwnd == iter->hwnd)
      {
        current_web_contents_ = iter->web_contents;
        SetParent(current_web_contents_->GetView()->GetNativeView(), window_);
        //current_web_contents_->GetController().Reload(false);
        CString str;
        str.Format(L"%u  %u\n", iter->hwnd, iter->web_contents);
        OutputDebugString(str);
      }
    }
    
  }

  void SimpleWebContentsDelegate::AddTab()
  {
    Initialize(current_web_contents_->GetBrowserContext(), GURL(), NULL, MSG_ROUTING_NONE, gfx::Size());
  }

  void SimpleWebContentsDelegate::CloseTab(LPARAM lParam)
  {
    HWND hwnd = (HWND)lParam;
    for (std::vector<TAB_INFO>::iterator iter = tab_info_.begin(); iter != tab_info_.end(); ++iter)
    {
      if (hwnd == iter->hwnd)
      {
        TAB_INFO ti = *iter;
        tab_info_.erase(iter);
        delete ti.web_contents;
        break;
      }
    }
    if (tab_info_.empty())
    {
      AddTab();
      return;
    }
    current_web_contents_ = tab_info_[0].web_contents;
  }

  const string16 SimpleWebContentsDelegate::GetURL()
  {
    GURL url;
    if (current_web_contents_)
    {
      url = current_web_contents_->GetURL();
    }
    std::string sTemp = url.spec();
    string16 sUrl(sTemp.begin(), sTemp.end());
    return sUrl;
  }
  
  const string16 SimpleWebContentsDelegate::GetTitle()
  {
    string16 title;
    if (current_web_contents_)
    {
      title = current_web_contents_->GetTitle();
    }
    return title;
  }

  TAB_INFO SimpleWebContentsDelegate::GetTabInfo(HWND hwnd)
  {
    TAB_INFO ti;
    for (std::vector<TAB_INFO>::iterator iter = tab_info_.begin(); iter != tab_info_.end(); ++iter)
    {
      if (hwnd == iter->hwnd)
      {
        
        ti = *iter;
        break;
      }
    }
    return ti;
  }

  void SimpleWebContentsDelegate::Back()
  {
    current_web_contents_->GetController().GoBack();
    current_web_contents_->GetView()->Focus();
  }

  void SimpleWebContentsDelegate::Forward()
  {
    current_web_contents_->GetController().GoForward();
    current_web_contents_->GetView()->Focus();
  }

  void SimpleWebContentsDelegate::Reload()
  {
    current_web_contents_->GetController().Reload(false);
    current_web_contents_->GetView()->Focus();
  }

  void SimpleWebContentsDelegate::Stop()
  {
    current_web_contents_->Stop();
    current_web_contents_->GetView()->Focus();
  }

}  // namespace content
