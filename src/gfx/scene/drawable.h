#ifndef gnat_gfx_scene_drawable_H
#define gnat_gfx_scene_drawable_H

#include "gnat.h"

namespace gnat {

class Node;
class Material;

/** Something that is drawn by the video card, attaches to scene nodes. */
class Drawable {
 public:
  Drawable();
  virtual ~Drawable();

  virtual void Draw() = 0;

  void SetMaterial(Material* material);
  Material* GetMaterial();

  Node* GetParent();

 private:

  void SetParent(Node* parent);

  Node* parent_;
  Material* material_;

  friend class Node;
};

}  // namespace gnat

#endif  // gnat_gfx_scene_drawable_H
