#ifndef gnat_gfx_scene_mesh_drawable_H
#define gnat_gfx_scene_mesh_drawable_H

#include "gnat.h"

#include "gfx/scene/drawable.h"

namespace gnat {

class Mesh;

// TODO: not super keen on the name, rethink?
class MeshDrawable : public Drawable {
 public:
  
  MeshDrawable(Mesh* mesh);

  virtual void Draw();

 private:

  Mesh* mesh_;
};

}  // namespace gnat

#endif  // gnat_gfx_scene_mesh_drawable_H
