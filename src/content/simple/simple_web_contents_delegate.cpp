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
static const int kTestWindowWidth = 800;
static const int kTestWindowHeight = 600;

namespace content {


  SimpleWebContentsDelegate::~SimpleWebContentsDelegate() {
  }


}  // namespace content
