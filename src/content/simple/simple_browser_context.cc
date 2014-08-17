// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/simple_browser_context.h"

#include "base/bind.h"
#include "base/command_line.h"
#include "base/environment.h"
#include "base/file_util.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "base/threading/thread.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/resource_context.h"
#include "content/public/browser/storage_partition.h"
#include "content/public/common/content_switches.h"
#include "content/simple/simple_url_request_context_getter.h"


namespace content {

  class SimpleBrowserContext::SimpleResourceContext : public ResourceContext {
  public:
    SimpleResourceContext() : getter_(NULL) {}
    virtual ~SimpleResourceContext() {}

    // ResourceContext implementation:
    virtual net::HostResolver* GetHostResolver() OVERRIDE {
      CHECK(getter_);
      return getter_->host_resolver();
    }
    virtual net::URLRequestContext* GetRequestContext() OVERRIDE {
      CHECK(getter_);
      return getter_->GetURLRequestContext();
    }

    void set_url_request_context_getter(SimpleURLRequestContextGetter* getter) {
      getter_ = getter;
    }

  private:
    SimpleURLRequestContextGetter* getter_;

    DISALLOW_COPY_AND_ASSIGN(SimpleResourceContext);
  };

SimpleBrowserContext::SimpleBrowserContext(bool off_the_record)
    : off_the_record_(off_the_record),
    ignore_certificate_errors_(false),
    resource_context_(new SimpleResourceContext){
}

SimpleBrowserContext::~SimpleBrowserContext() {
}

base::FilePath SimpleBrowserContext::GetPath() {
  return path_;
}

bool SimpleBrowserContext::IsOffTheRecord() const {
  return off_the_record_;
}

DownloadManagerDelegate* SimpleBrowserContext::GetDownloadManagerDelegate()  {

  return NULL;
}

net::URLRequestContextGetter* SimpleBrowserContext::GetRequestContext()  {
  return GetDefaultStoragePartition(this)->GetURLRequestContext();
}

net::URLRequestContextGetter* SimpleBrowserContext::GetRequestContextForRenderProcess(int renderer_child_id)  {
    return GetRequestContext();
}

net::URLRequestContextGetter* SimpleBrowserContext::GetMediaRequestContext()  {
    return GetRequestContext();
}

net::URLRequestContextGetter* SimpleBrowserContext::GetMediaRequestContextForRenderProcess(int renderer_child_id)  {
    return GetRequestContext();
}

net::URLRequestContextGetter* SimpleBrowserContext::GetMediaRequestContextForStoragePartition(const base::FilePath& partition_path,bool in_memory) {
    return GetRequestContext();
}

ResourceContext* SimpleBrowserContext::GetResourceContext()  {
  return resource_context_.get();
}

GeolocationPermissionContext* SimpleBrowserContext::GetGeolocationPermissionContext()  {
    return NULL;
}

SpeechRecognitionPreferences* SimpleBrowserContext::GetSpeechRecognitionPreferences() {
    return NULL;
}

quota::SpecialStoragePolicy* SimpleBrowserContext::GetSpecialStoragePolicy() {
  return NULL;
}

net::URLRequestContextGetter* SimpleBrowserContext::CreateRequestContext(ProtocolHandlerMap* protocol_handlers) {
    DCHECK(!url_request_getter_);
    url_request_getter_ = new SimpleURLRequestContextGetter(
      ignore_certificate_errors_,GetPath(),
      BrowserThread::UnsafeGetMessageLoopForThread(BrowserThread::IO),
      BrowserThread::UnsafeGetMessageLoopForThread(BrowserThread::FILE),
      protocol_handlers);
    resource_context_->set_url_request_context_getter(url_request_getter_.get());
    return url_request_getter_.get();
}

net::URLRequestContextGetter* SimpleBrowserContext::CreateRequestContextForStoragePartition(
  const base::FilePath& partition_path, bool in_memory, ProtocolHandlerMap* protocol_handlers) {
    return NULL;
}

}  // namespace content
