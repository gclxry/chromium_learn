// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/simple_web_contents_view_delegate.h"

#include "base/command_line.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/browser/render_widget_host_view.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_view.h"
#include "content/public/common/context_menu_params.h"
#include "content/simple/simple_web_contents_view_delegate_creator.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebContextMenuData.h"

using WebKit::WebContextMenuData;


namespace content {

WebContentsViewDelegate* CreateSimpleWebContentsViewDelegate(
    WebContents* web_contents) {
  return new SimpleWebContentsViewDelegate(web_contents);
}

SimpleWebContentsViewDelegate::SimpleWebContentsViewDelegate(
    WebContents* web_contents)
    : web_contents_(web_contents) {
}

SimpleWebContentsViewDelegate::~SimpleWebContentsViewDelegate() {
}

void SimpleWebContentsViewDelegate::ShowContextMenu(
  const ContextMenuParams& params,
  ContextMenuSourceType type) {
}

WebDragDestDelegate* SimpleWebContentsViewDelegate::GetDragDestDelegate() {
  return NULL;
}

void SimpleWebContentsViewDelegate::StoreFocus() {
}

void SimpleWebContentsViewDelegate::RestoreFocus() {
}

bool SimpleWebContentsViewDelegate::Focus() {
  return false;
}

void SimpleWebContentsViewDelegate::TakeFocus(bool reverse) {
}

void SimpleWebContentsViewDelegate::SizeChanged(const gfx::Size& size) {
}

}  // namespace content
