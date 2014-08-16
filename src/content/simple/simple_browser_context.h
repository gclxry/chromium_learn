// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SIMPLE_SIMPLE_BROWSER_CONTEXT_H_
#define CONTENT_SIMPLE_SIMPLE_BROWSER_CONTEXT_H_

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/content_browser_client.h"
#include "net/url_request/url_request_job_factory.h"

namespace content {

class DownloadManagerDelegate;
class ResourceContext;

// 存放浏览器回话上下文信息
class SimpleBrowserContext : public BrowserContext {
public:
  explicit SimpleBrowserContext(bool off_the_record);
  virtual ~SimpleBrowserContext();


private:
  bool off_the_record_;

  DISALLOW_COPY_AND_ASSIGN(SimpleBrowserContext);
};

}  // namespace content

#endif  // CONTENT_SIMPLE_SIMPLE_BROWSER_CONTEXT_H_
