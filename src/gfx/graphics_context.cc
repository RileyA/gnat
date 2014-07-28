#include "gfx/graphics_context.h"

#include "platform/gl_platform_context.h"
#include "gfx/scene/mesh.h"
#include "gfx/material/texture.h"
#include "gfx/material/material.h"
#include "gl.h"

#include "third_party/oyster/include/Oyster.h"

namespace gnat {

GraphicsContext::GraphicsContext(GLPlatformContext* platform_context)
  : platform_context_(platform_context) {
}
//---------------------------------------------------------------------------

GraphicsContext::~GraphicsContext() {
  for (Map<String, Mesh *>::iterator it = meshes_.begin(); it != meshes_.end();
       ++it) {
    delete it->second;
  }
  for (Map<String, Texture *>::iterator it = textures_.begin();
       it != textures_.end(); ++it) {
    delete it->second;
  }
  for (Map<String, Material *>::iterator it = materials_.begin();
       it != materials_.end(); ++it) {
    delete it->second;
  }
}
//---------------------------------------------------------------------------

void GraphicsContext::Init() {
  oyster_ = new Oyster::Oyster(platform_context_->width(),
                               platform_context_->height());
  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, platform_context_->width(), platform_context_->height());
}
//---------------------------------------------------------------------------

void GraphicsContext::Deinit() {
}
//---------------------------------------------------------------------------

void GraphicsContext::Update(Real delta) {
  // Clear and ensure we have the right viewport
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // TODO listen for resized event from |platform_context_|
  //glViewport(0, 0, platform_context_->width(), platform_context_->height());

  RenderFrame(camera_);

  // Swap buffers/etc.
  platform_context_->UpdateDisplay();
}
//---------------------------------------------------------------------------

void GraphicsContext::RenderFrame(Camera* camera, Node* root) {
  if (!root)
    root = &root_;
  scene_manager_.Draw(root, camera);
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

Mesh* GraphicsContext::GetMesh(String filename) {
  if (meshes_.count(filename))
    return meshes_[filename];
  Mesh* out = Mesh::Load(filename);
  if (out)
    return meshes_[filename] = out;
}
//---------------------------------------------------------------------------

Texture* GraphicsContext::GetTexture(String filename, bool alpha) {
  if (textures_.count(filename))
    return textures_[filename];
  Texture* out = Texture::Load(filename, alpha);
  if (out)
    return textures_[filename] = out;
}
//---------------------------------------------------------------------------

Material* GraphicsContext::GetMaterial(String filename) {
  if (materials_.count(filename))
    return materials_[filename];
  Material* out = Material::FromFile(this, filename);
  if (out)
    return materials_[filename] = out;
}
//---------------------------------------------------------------------------

}  // namespace gnat
