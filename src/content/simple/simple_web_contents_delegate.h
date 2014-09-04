// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CONTENT_SIMPLE_SIMPLE_WEB_CONTENTS_DELEGATE_H_
#define CONTENT_SIMPLE_SIMPLE_WEB_CONTENTS_DELEGATE_H_

#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_delegate.h"
#include "ipc/ipc_channel.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"

//bool AddWebContentsDelegate(content::SimpleWebContentsDelegate* web_content_delegate);

class GURL;
//class content::SimpleWebContentsDelegate;
namespace content {

  class BrowserContext;
  class SiteInstance;
  class WebContents;

  struct TAB_INFO
  {
    WebContents* web_contents;
    string16 title;
    HWND hwnd;
  };

  // This represents one window of the Content Shell, i.e. all the UI including
  // buttons and url bar, as well as the web content area.
  class SimpleWebContentsDelegate : public WebContentsDelegate,
    public NotificationObserver {
  public:
    virtual ~SimpleWebContentsDelegate();

    // 现有WebContents中从地址栏中打开url
    void LoadURL(const GURL& url);

    void SetBrowserContext(BrowserContext* browser_context);

    // Do one time initialization at application startup.
    void Initialize(BrowserContext* browser_context,
      const GURL& url,
      SiteInstance* site_instance,
      int routing_id,
      const gfx::Size& initial_size);

    void SetHWND(HWND main_window, HWND client_window);
    void MakePair(HWND button_hwnd, LPARAM lParam);
    void SwitchTab(HWND hwnd);
    void AddTab();
    void CloseTab(LPARAM lParam);
    const string16 GetURL();
    const string16 GetTitle();
    TAB_INFO GetTabInfo(HWND hwnd);
    void Back();
    void Forward();
    void Reload();
    void Stop();



    // WebContentsDelegate
    // 浏览器内部自己处理的跳转，创建一个新的SimpleWebContentsDelegate
    virtual void WebContentsCreated(WebContents* source_contents,
      int64 source_frame_id,
      const string16& frame_name,
      const GURL& target_url,
      WebContents* new_contents) OVERRIDE;

    virtual void DidNavigateMainFramePostCommit(
      WebContents* web_contents) OVERRIDE;

    // NotificationObserver
    virtual void Observe(int type,
      const NotificationSource& source,
      const NotificationDetails& details) OVERRIDE;

  private:
  public:
    explicit SimpleWebContentsDelegate();

  public:
    //scoped_ptr<WebContents> web_contents_;
    HWND window_;
    HWND main_window_;
    std::vector<TAB_INFO> tab_info_;
    WebContents* current_web_contents_;
    BrowserContext* browser_context_;

    // Notification manager
    NotificationRegistrar registrar_;
  };

}  // namespace content

#endif  // CONTENT_SIMPLE_SIMPLE_WEB_CONTENTS_DELEGATE_H_
