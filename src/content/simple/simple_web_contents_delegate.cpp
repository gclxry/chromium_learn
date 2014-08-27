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



// Content area size for newly created windows.
static const int kTestWindowWidth = 1420;
static const int kTestWindowHeight = 750;

std::vector<content::SimpleWebContentsDelegate*> g_WebContentsDelegate;
HWND g_hWnd = NULL;

bool AddWebContentsDelegate(content::SimpleWebContentsDelegate* web_content_delegate)
{
  g_WebContentsDelegate.push_back(web_content_delegate);
  return true;
}

namespace content {

  SimpleWebContentsDelegate::SimpleWebContentsDelegate(){
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

  void SimpleWebContentsDelegate::CreateNew(BrowserContext* browser_context,
    const GURL& url, SiteInstance* site_instance, int routing_id, const gfx::Size& initial_size) {
      WebContents::CreateParams create_params(browser_context, site_instance);
      create_params.routing_id = routing_id;
      if (!initial_size.IsEmpty())
        create_params.initial_size = initial_size;
      else
        create_params.initial_size = gfx::Size(kTestWindowWidth, kTestWindowHeight);
      WebContents* web_contents = WebContents::Create(create_params);
      web_contents_.reset(web_contents);
      web_contents->SetDelegate(this);
      SetParent(web_contents_->GetView()->GetNativeView(), window_);
      
      if (!url.is_empty())
      {
        LoadURL(url);
      }
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

  void SimpleWebContentsDelegate::WebContentsCreated(WebContents* source_contents,
    int64 source_frame_id,
    const string16& frame_name,
    const GURL& target_url,
    WebContents* new_contents) {

      SimpleWebContentsDelegate* new_tab = new content::SimpleWebContentsDelegate();
      
      new_tab->SetHWND(g_hWnd);

      new_tab->web_contents_.reset(new_contents);
      new_tab->web_contents_->SetDelegate(new_tab);
      new_tab->window_ = window_;
      SetParent(new_tab->web_contents_->GetView()->GetNativeView(), new_tab->window_);


      AddWebContentsDelegate(new_tab);

  }

  void SimpleWebContentsDelegate::SetHWND(HWND window)
  {
    //window_ = window;
    g_hWnd = window;
  }

  void SimpleWebContentsDelegate::DidNavigateMainFramePostCommit(WebContents* web_contents) {
    GURL url = web_contents_->GetURL();
    int ia = 0;
    ia++;
  }

}  // namespace content
