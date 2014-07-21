// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SIMPLE_SIMPLE_DOWNLOAD_MANAGER_DELEGATE_H_
#define CONTENT_SIMPLE_SIMPLE_DOWNLOAD_MANAGER_DELEGATE_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "content/public/browser/download_manager_delegate.h"

namespace content {

class DownloadManager;

class SimpleDownloadManagerDelegate
    : public DownloadManagerDelegate,
      public base::RefCountedThreadSafe<SimpleDownloadManagerDelegate> {
 public:
  SimpleDownloadManagerDelegate();

  void SetDownloadManager(DownloadManager* manager);

  virtual void Shutdown() OVERRIDE;
  virtual bool DetermineDownloadTarget(
      DownloadItem* download,
      const DownloadTargetCallback& callback) OVERRIDE;
  virtual bool ShouldOpenDownload(
      DownloadItem* item,
      const DownloadOpenDelayedCallback& callback) OVERRIDE;

  // Inhibits prompting and sets the default download path.
  void SetDownloadBehaviorForTesting(
      const base::FilePath& default_download_path);

 protected:
  // To allow subclasses for testing.
  virtual ~SimpleDownloadManagerDelegate();

 private:
  friend class base::RefCountedThreadSafe<SimpleDownloadManagerDelegate>;


  void GenerateFilename(int32 download_id,
                        const DownloadTargetCallback& callback,
                        const base::FilePath& generated_name,
                        const base::FilePath& suggested_directory);
  void OnDownloadPathGenerated(int32 download_id,
                               const DownloadTargetCallback& callback,
                               const base::FilePath& suggested_path);
  void ChooseDownloadPath(int32 download_id,
                          const DownloadTargetCallback& callback,
                          const base::FilePath& suggested_path);

  DownloadManager* download_manager_;
  base::FilePath default_download_path_;
  bool suppress_prompting_;

  DISALLOW_COPY_AND_ASSIGN(SimpleDownloadManagerDelegate);
};

}  // namespace content

#endif  // CONTENT_SIMPLE_SIMPLE_DOWNLOAD_MANAGER_DELEGATE_H_
