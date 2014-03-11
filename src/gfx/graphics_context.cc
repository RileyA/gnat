#include "gfx/graphics_context.h"

#include "platform/gl_platform_context.h"
#include "gl.h"

#include <cstdio>

namespace gnat {

GraphicsContext::GraphicsContext(GLPlatformContext* platform_context)
  : platform_context_(platform_context) {
}

GraphicsContext::~GraphicsContext() {
  
}

void GraphicsContext::Update(Real delta) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  platform_context_->UpdateDisplay();
}

}  // namespace gnat
