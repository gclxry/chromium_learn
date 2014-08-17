// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CONTENT_SHELL_SHELL_H_
#define CONTENT_SHELL_SHELL_H_


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


namespace views {
  class Widget;
  class ViewsDelegate;
}


class GURL;
namespace content {

class BrowserContext;
//class ShellDevToolsFrontend;
//class ShellJavaScriptDialogManager;
class SiteInstance;
class WebContents;

// This represents one window of the Content Shell, i.e. all the UI including
// buttons and url bar, as well as the web content area.
class Shell : public WebContentsDelegate,
              public NotificationObserver {
 public:
  virtual ~Shell();

  void LoadURL(const GURL& url);
  void LoadURLForFrame(const GURL& url, const std::string& frame_name);
  void GoBackOrForward(int offset);
  void Reload();
  void Stop();
  void UpdateNavigationControls();
  void Close();
  void ShowDevTools();
  void CloseDevTools();

  // Resizes the main window to the given dimensions.
  void SizeTo(int width, int height);


  // Do one time initialization at application startup.
  static void Initialize();

  static Shell* CreateNewWindow(BrowserContext* browser_context,
                                const GURL& url,
                                SiteInstance* site_instance,
                                int routing_id,
                                const gfx::Size& initial_size);

  // Returns the Shell object corresponding to the given RenderViewHost.
  static Shell* FromRenderViewHost(RenderViewHost* rvh);

  // Returns the currently open windows.
  static std::vector<Shell*>& windows() { return windows_; }

  // Closes all windows and returns. This runs a message loop.
  static void CloseAllWindows();

  // Closes all windows and exits.
  static void PlatformExit();

  // Used for content_browsertests. Called once.
  static void SetShellCreatedCallback(
      base::Callback<void(Shell*)> shell_created_callback);

  WebContents* web_contents() const { return web_contents_.get(); }
  gfx::NativeWindow window() { return window_; }


  // WebContentsDelegate
  virtual WebContents* OpenURLFromTab(WebContents* source,
                                      const OpenURLParams& params) OVERRIDE;
  virtual void LoadingStateChanged(WebContents* source) OVERRIDE;

  virtual void ToggleFullscreenModeForTab(WebContents* web_contents,
                                          bool enter_fullscreen) OVERRIDE;
  virtual bool IsFullscreenForTabOrPending(
      const WebContents* web_contents) const OVERRIDE;
  virtual void RequestToLockMouse(WebContents* web_contents,
                                  bool user_gesture,
                                  bool last_unlocked_by_target) OVERRIDE;
  virtual void CloseContents(WebContents* source) OVERRIDE;
  virtual bool CanOverscrollContent() const OVERRIDE;
  virtual void WebContentsCreated(WebContents* source_contents,
                                  int64 source_frame_id,
                                  const string16& frame_name,
                                  const GURL& target_url,
                                  WebContents* new_contents) OVERRIDE;
  virtual void DidNavigateMainFramePostCommit(
      WebContents* web_contents) OVERRIDE;
  //virtual JavaScriptDialogManager* GetJavaScriptDialogManager() OVERRIDE;

  //virtual bool AddMessageToConsole(WebContents* source,
  //                                 int32 level,
  //                                 const string16& message,
  //                                 int32 line_no,
  //                                 const string16& source_id) OVERRIDE;
  //virtual void RendererUnresponsive(WebContents* source) OVERRIDE;
  virtual void ActivateContents(WebContents* contents) OVERRIDE;
  virtual void DeactivateContents(WebContents* contents) OVERRIDE;

 private:
  enum UIControl {
    BACK_BUTTON,
    FORWARD_BUTTON,
    STOP_BUTTON
  };

  explicit Shell(WebContents* web_contents);

  // Helper to create a new Shell given a newly created WebContents.
  static Shell* CreateShell(WebContents* web_contents);

  // Helper for one time initialization of application
  static void PlatformInitialize(const gfx::Size& default_window_size);

  // All the methods that begin with Platform need to be implemented by the
  // platform specific Shell implementation.
  // Called from the destructor to let each platform do any necessary cleanup.
  void PlatformCleanUp();
  // Creates the main window GUI.
  void PlatformCreateWindow(int width, int height);
  // Links the WebContents into the newly created window.
  void PlatformSetContents();
  // Resize the content area and GUI.
  void PlatformResizeSubViews();
  // Enable/disable a button.
  void PlatformEnableUIControl(UIControl control, bool is_enabled);
  // Updates the url in the url bar.
  void PlatformSetAddressBarURL(const GURL& url);
  // Sets whether the spinner is spinning.
  void PlatformSetIsLoading(bool loading);
  // Set the title of shell window
  void PlatformSetTitle(const string16& title);

  gfx::NativeView GetContentView();

  // NotificationObserver
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  static ATOM RegisterWindowClass();
  static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
  static LRESULT CALLBACK EditWndProc(HWND, UINT, WPARAM, LPARAM);


  scoped_ptr<WebContents> web_contents_;

  bool is_fullscreen_;

  gfx::NativeWindow window_;
  gfx::NativeEditView url_edit_view_;

  // Notification manager
  NotificationRegistrar registrar_;


  WNDPROC default_edit_wnd_proc_;
  static HINSTANCE instance_handle_;

  static views::ViewsDelegate* views_delegate_;

  views::Widget* window_widget_;

  bool headless_;

  // A container of all the open windows. We use a vector so we can keep track
  // of ordering.
  static std::vector<Shell*> windows_;

  static base::Callback<void(Shell*)> shell_created_callback_;

  // True if the destructur of Shell should post a quit closure on the current
  // message loop if the destructed Shell object was the last one.
  static bool quit_message_loop_;
};

}  // namespace content

#endif  // CONTENT_SHELL_SHELL_H_
