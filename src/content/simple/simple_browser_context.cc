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


namespace content {


SimpleBrowserContext::SimpleBrowserContext(bool off_the_record)
    : off_the_record_(off_the_record){
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
  return NULL;
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

}  // namespace content
