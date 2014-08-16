// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SIMPLE_SIMPLE_CONTENT_CLIENT_H_
#define CONTENT_SIMPLE_SIMPLE_CONTENT_CLIENT_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "content/public/common/content_client.h"

namespace content {

// 逻辑意义上的浏览器整体对象
class SimpleContentClient : public ContentClient {
 public:
  virtual ~SimpleContentClient();

};

}  // namespace content

#endif  // CONTENT_SIMPLE_SSIMPLE_CONTENT_CLIENT_H_
