// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SIMPLE_SIMPLE_WEB_CONTENTS_VIEW_DELEGATE_H_
#define CONTENT_SIMPLE_SIMPLE_WEB_CONTENTS_VIEW_DELEGATE_H_

#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_view_delegate.h"
#include "content/public/common/context_menu_params.h"
#include "content/public/common/context_menu_source_type.h"

namespace content {

class SimpleWebContentsViewDelegate : public WebContentsViewDelegate {
 public:
  explicit SimpleWebContentsViewDelegate(WebContents* web_contents);
  virtual ~SimpleWebContentsViewDelegate();

  // Overridden from WebContentsViewDelegate:
  virtual void ShowContextMenu(const ContextMenuParams& params, ContextMenuSourceType type) OVERRIDE;
  virtual WebDragDestDelegate* GetDragDestDelegate() OVERRIDE;
  virtual void StoreFocus() OVERRIDE;
  virtual void RestoreFocus() OVERRIDE;
  virtual bool Focus() OVERRIDE;
  virtual void TakeFocus(bool reverse) OVERRIDE;
  virtual void SizeChanged(const gfx::Size& size) OVERRIDE;

private:
  WebContents* web_contents_;

  DISALLOW_COPY_AND_ASSIGN(SimpleWebContentsViewDelegate);
};

}  // namespace content

#endif // CONTENT_SIMPLE_SIMPLE_WEB_CONTENTS_VIEW_DELEGATE_H_
