// Copyright (c) 2011 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXAMPLES_TUMBLER_TUMBLER_H_
#define EXAMPLES_TUMBLER_TUMBLER_H_

#include <pthread.h>
#include <map>
#include <vector>

#include <ppapi/cpp/instance.h>

#include "platform/nacl/opengl_context.h"
#include "platform/nacl/opengl_context_ptrs.h"

namespace lol {

class NaClInstance : public pp::Instance {
 public:
  explicit NaClInstance(PP_Instance instance);

  // The dtor makes the 3D context current before deleting the cube view, then
  // destroys the 3D context both in the module and in the browser.
  virtual ~NaClInstance();

  // Called by the browser when the NaCl module is loaded and all ready to go.
  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);

  // Called whenever the in-browser window changes size.
  virtual void DidChangeView(const pp::Rect& position, const pp::Rect& clip);

  // Called by the browser to handle the postMessage() call in Javascript.
  virtual void HandleMessage(const pp::Var& message);

  // Bind and publish the module's methods to JavaScript.
  //void InitializeMethods(ScriptingBridge* bridge);

  // Called to draw the contents of the module's browser area.
  void DrawSelf();

// private:
  // Browser connectivity and scripting support.
//  ScriptingBridge scripting_bridge_;

  SharedOpenGLContext opengl_context_;

    ivec2 m_size;
};

}  // namespace lol

#endif  // EXAMPLES_TUMBLER_TUMBLER_H_
