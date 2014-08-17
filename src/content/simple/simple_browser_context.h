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
class SimpleURLRequestContextGetter;

// 存放浏览器会话上下文信息
class SimpleBrowserContext : public BrowserContext {
public:
  explicit SimpleBrowserContext(bool off_the_record);
  virtual ~SimpleBrowserContext();

  // BrowserContext implementation.
  virtual base::FilePath GetPath() OVERRIDE;
  virtual bool IsOffTheRecord() const OVERRIDE;
  virtual DownloadManagerDelegate* GetDownloadManagerDelegate() OVERRIDE;
  virtual net::URLRequestContextGetter* GetRequestContext() OVERRIDE;
  virtual net::URLRequestContextGetter* GetRequestContextForRenderProcess(int renderer_child_id) OVERRIDE;
  virtual net::URLRequestContextGetter* GetMediaRequestContext() OVERRIDE;
  virtual net::URLRequestContextGetter* GetMediaRequestContextForRenderProcess(int renderer_child_id) OVERRIDE;
  virtual net::URLRequestContextGetter* GetMediaRequestContextForStoragePartition(const base::FilePath& partition_path, bool in_memory) OVERRIDE;
  virtual ResourceContext* GetResourceContext() OVERRIDE;
  virtual GeolocationPermissionContext* GetGeolocationPermissionContext() OVERRIDE;
  virtual SpeechRecognitionPreferences* GetSpeechRecognitionPreferences() OVERRIDE;
  virtual quota::SpecialStoragePolicy* GetSpecialStoragePolicy() OVERRIDE;

  net::URLRequestContextGetter* CreateRequestContext(ProtocolHandlerMap* protocol_handlers);
  net::URLRequestContextGetter* CreateRequestContextForStoragePartition(
    const base::FilePath& partition_path, bool in_memory, ProtocolHandlerMap* protocol_handlers);

private:
  class SimpleResourceContext;
  bool off_the_record_;
  base::FilePath path_;
  bool ignore_certificate_errors_;
  scoped_ptr<SimpleResourceContext> resource_context_;
  scoped_refptr<SimpleURLRequestContextGetter> url_request_getter_;

  DISALLOW_COPY_AND_ASSIGN(SimpleBrowserContext);
};

}  // namespace content

#endif  // CONTENT_SIMPLE_SIMPLE_BROWSER_CONTEXT_H_
