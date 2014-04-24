#ifndef gnat_gfx_graphics_context_H
#define gnat_gfx_graphics_context_H

#include "gnat.h"
#include "event/event_handler.h"
#include "gfx/scene/camera.h"
#include "gfx/scene/node.h"
#include "gfx/scene/scene_manager.h"
#include "util/color.h"

namespace gnat {

class GLPlatformContext;
class Mesh;

class GraphicsContext : public EventHandler {
 public:

  GraphicsContext(GLPlatformContext* platform_context);
  ~GraphicsContext();

  void Init();
  void Deinit();

  void Update(Real delta);
  void RenderFrame(Camera* camera);

  void SetClearColor(ColorF32 color);

  void SetMainCamera(Camera* camera);

  Mesh* LoadMesh(String filename);
  //Texture* LoadTexture(String filename);

  Node* GetRootNode() { return &root_; }

 private:
  
  Map<String, Mesh*> meshes_;
  //Map<String, Texture*> textures_;

  GLPlatformContext* platform_context_;

  // The active camera.
  Camera* camera_;

  // Root scene node.
  Node root_;

  SceneManager scene_manager_;
};

}  // namespace gnat

#endif  // gnat_gfx_graphics_context_H
