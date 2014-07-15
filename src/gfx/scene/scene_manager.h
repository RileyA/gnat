#ifndef gnat_gfx_scene_scene_manager_H
#define gnat_gfx_scene_scene_manager_H


#include "gnat.h"

#include "gfx/scene/node.h"

namespace gnat {

class Drawable;
class Material;
class Camera;

class SceneManager : public Node::Visitor {
 public:
  SceneManager();
  virtual ~SceneManager();

  void Draw(Node* root, Camera* camera);

  void ConfigureMaterial(Material* material, Camera* camera);
  void ConfigureDrawable(Drawable* draw, Camera* camera);

 private:

  virtual void Visit(Node* n);

  typedef List<Drawable*> DrawQueue;
  typedef Map<Material*,  DrawQueue> DrawMap;
  typedef Map<int, DrawMap> DrawGroupMap;

  DrawGroupMap draws_;
};

}  // namespace gnat

#endif  // gnat_gfx_scene_scene_manager_H
