// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/simple_content_client.h"

#include "base/command_line.h"
#include "base/strings/string_piece.h"
#include "base/utf_string_conversions.h"
#include "content/public/common/content_switches.h"
#include "grit/webkit_resources.h"
#include "grit/webkit_strings.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "webkit/user_agent/user_agent_util.h"

namespace content {

SimpleContentClient::~SimpleContentClient() {
}

}  // namespace content
