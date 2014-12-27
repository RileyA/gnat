#ifndef gnat_gfx_graphics_context_H
#define gnat_gfx_graphics_context_H

#include "gnat.h"
#include "event/event_handler.h"
#include "gfx/scene/camera.h"
#include "gfx/scene/node.h"
#include "gfx/scene/scene_manager.h"
#include "platform/gl_platform_context.h"
#include "util/color.h"

namespace Oyster {
class Oyster;
}

namespace gnat {

class GLPlatformContext;
class Mesh;
class Texture;
class Material;
class FramebufferObject;

class GraphicsContext : public EventHandler {
 public:

  GraphicsContext(GLPlatformContext* platform_context);
  ~GraphicsContext();

  void Init();
  void Deinit();

  void Update(Real delta);
  void RenderFrame(Camera* camera, Node* root = NULL);

  void SetClearColor(ColorF32 color);

  void SetMainCamera(Camera* camera);

  Mesh* GetMesh(String filename);
  Texture* GetTexture(String filename, bool alpha);
  Material* GetMaterial(String filename);

  Node* GetRootNode() { return &root_; }

  Oyster::Oyster* oyster() { return oyster_; }

  int width() { return platform_context_->width(); }
  int height() { return platform_context_->height(); }

 private:
  
  Map<String, Mesh*> meshes_;
  Map<String, Texture*> textures_;
  Map<String, Material*> materials_;

  GLPlatformContext* platform_context_;

  // The active camera.
  Camera* camera_;

  // Root scene node.
  Node root_;

  SceneManager scene_manager_;

  Oyster::Oyster* oyster_;
};

}  // namespace gnat

#endif  // gnat_gfx_graphics_context_H
