// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SIMPLE_SIMPLE_MAIN_DELEGATE_H_
#define CONTENT_SIMPLE_SIMPLE_MAIN_DELEGATE_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/app/content_main_delegate.h"

namespace content {

class SimpleMainDelegate : public ContentMainDelegate {
 public:
  SimpleMainDelegate();
  virtual ~SimpleMainDelegate();

  // ContentMainDelegate implementation:

  // Tells the embedder that the absolute basic startup has been done, i.e.
  // it's now safe to create singletons and check the command line. Return true
  // if the process should exit afterwards, and if so, |exit_code| should be
  // set. This is the place for embedder to do the things that must happen at
  // the start. Most of its startup code should be in the methods below.
  virtual bool BasicStartupComplete(int* exit_code) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(SimpleMainDelegate);
};

}  // namespace content

#endif  // CONTENT_SIMPLE_SIMPLE_MAIN_DELEGATE_H_
