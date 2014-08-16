// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SIMPLE_SIMPLE_CONTENT_BROWSER_CLIENT_H_
#define CONTENT_SIMPLE_SIMPLE_CONTENT_BROWSER_CLIENT_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "content/public/browser/content_browser_client.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"


namespace content {

class SimpleBrowserMainParts;
class SimpleBrowserContext;

// browser½ø³ÌµÄÂß¼­
class SimpleContentBrowserClient : public ContentBrowserClient,
                                  public NotificationObserver {
public:
  SimpleContentBrowserClient();
  virtual ~SimpleContentBrowserClient();

  // ContentBrowserClient overrides.
  virtual BrowserMainParts* CreateBrowserMainParts(const MainFunctionParams& parameters) OVERRIDE;

  // NotificationObserver implementation.
  virtual void Observe(int type, const NotificationSource& source, const NotificationDetails& details) OVERRIDE;

private:
  SimpleBrowserMainParts* simple_browser_main_parts_;
};

}  // namespace content

#endif  // CONTENT_SIMPLE_SIMPLE_CONTENT_BROWSER_CLIENT_H_
