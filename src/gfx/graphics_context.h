#ifndef gnat_gfx_graphics_context_H
#define gnat_gfx_graphics_context_H

#include "gnat.h"
#include "event/event_handler.h"
#include "gfx/material/material_manager.h"
#include "gfx/scene/node.h"
#include "gfx/scene/camera.h"
#include "util/color.h"

namespace gnat {

class GLPlatformContext;

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

  Node* GetRootNode() { return &root_; }

  Material* GetMaterial(String name);
  bool HasMaterial(String name);
  void AddMaterial(Material* material);

 private:

  GLPlatformContext* platform_context_;

  // The active camera.
  Camera* camera_;

  // Root scene node.
  Node root_;

  // Materials.
  Map<String, Material> materials_;
};

}  // namespace gnat

#endif  // gnat_gfx_graphics_context_H
