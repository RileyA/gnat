#include "gfx/graphics_context.h"

#include "platform/gl_platform_context.h"
#include "gl.h"

namespace gnat {

GraphicsContext::GraphicsContext(GLPlatformContext* platform_context)
  : platform_context_(platform_context) {
}
//---------------------------------------------------------------------------

GraphicsContext::~GraphicsContext() {
  
}
//---------------------------------------------------------------------------

void GraphicsContext::Init() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}
//---------------------------------------------------------------------------

void GraphicsContext::Deinit() {
}
//---------------------------------------------------------------------------

void GraphicsContext::Update(Real delta) {
  // Clear and ensure we have the right viewport
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // TODO listen for resized event from platform_context_
  glViewport(0, 0, platform_context_->width(), platform_context_->height());

  RenderFrame(camera_);

  // Swap buffers/etc.
  platform_context_->UpdateDisplay();
}
//---------------------------------------------------------------------------

void GraphicsContext::RenderFrame(Camera* camera) {
  // TODO Set up matrices
}
//---------------------------------------------------------------------------

void GraphicsContext::SetClearColor(ColorF32 color) {
  glClearColor(color.r, color.g, color.b, color.a);
}
//---------------------------------------------------------------------------

void GraphicsContext::SetMainCamera(Camera* camera) {
  camera_ = camera;
}
//---------------------------------------------------------------------------

}  // namespace gnat
