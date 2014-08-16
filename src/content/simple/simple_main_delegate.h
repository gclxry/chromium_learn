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


  DISALLOW_COPY_AND_ASSIGN(SimpleMainDelegate);
};

}  // namespace content

#endif  // CONTENT_SIMPLE_SIMPLE_MAIN_DELEGATE_H_
