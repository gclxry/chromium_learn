// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SIMPLE_SIMPLE_MAIN_DELEGATE_H_
#define CONTENT_SIMPLE_SIMPLE_MAIN_DELEGATE_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/simple/simple_content_client.h"
#include "content/public/app/content_main_delegate.h"

namespace content {
class SimpleContentBrowserClient;
//class ShellContentRendererClient;



// 自定义的浏览器从这里开始
class SimpleMainDelegate : public ContentMainDelegate {
 public:
  SimpleMainDelegate();
  virtual ~SimpleMainDelegate();

  // ContentMainDelegate implementation:
  virtual bool BasicStartupComplete(int* exit_code) OVERRIDE;
  virtual void PreSandboxStartup() OVERRIDE;
  virtual int RunProcess(
      const std::string& process_type,
      const MainFunctionParams& main_function_params) OVERRIDE;
  virtual ContentBrowserClient* CreateContentBrowserClient() OVERRIDE;
  //virtual ContentRendererClient* CreateContentRendererClient() OVERRIDE;

  static void InitializeResourceBundle();

 private:
  scoped_ptr<SimpleContentBrowserClient> browser_client_;
  //scoped_ptr<ShellContentRendererClient> renderer_client_;
  SimpleContentClient content_client_;

  DISALLOW_COPY_AND_ASSIGN(SimpleMainDelegate);
};

}  // namespace content

#endif  // CONTENT_SIMPLE_SIMPLE_MAIN_DELEGATE_H_
