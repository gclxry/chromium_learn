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


class GURL;
namespace content {

  class BrowserContext;
  class SiteInstance;
  class WebContents;

  // This represents one window of the Content Shell, i.e. all the UI including
  // buttons and url bar, as well as the web content area.
  class SimpleWebContentsDelegate : public WebContentsDelegate,
    public NotificationObserver {
  public:
    virtual ~SimpleWebContentsDelegate();
  };

}  // namespace content

#endif  // CONTENT_SIMPLE_SIMPLE_WEB_CONTENTS_DELEGATE_H_
