// Copyright (c) 2011 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

// A convenience wrapper for a shared OpenGLContext pointer type.  As other
// smart pointer types are needed, add them here.

#include <tr1/memory>

namespace lol {

class OpenGLContext;

typedef std::tr1::shared_ptr<OpenGLContext> SharedOpenGLContext;

}  // namespace lol

