#ifndef gnat_gfx_scene_node_H
#define gnat_gfx_scene_node_H

#include "gnat.h"

#include "gfx/math/matrix4.h"
#include "gfx/math/quaternion.h"
#include "gfx/math/vector3.h"

namespace gnat {

class Drawable;

/** A node in the gfx scene graph */
class Node {
 public:
  
  Node();
  virtual ~Node();

  void AddChild(Node* node);
  bool HasChild(Node* node);
  void RemoveChild(Node* node);
  void RemoveAllChildren();
  int GetNumChildren();

  void AddDrawable(Drawable* drawable);
  void RemoveDrawable(Drawable* drawable);
  bool HasDrawable(Drawable* drawable);
  void RemoveAllDrawables();
  int GetNumDrawables();

  List<Drawable*>::iterator GetDrawables() { return drawables_.begin(); }

  void SetPosition(const Vector3& position);
  void SetOrientation(const Quaternion& orientation);
  Vector3 GetPosition() { return position_; }
  Quaternion GetOrientation() { return orientation_; }

  class Visitor {
   public:
    virtual ~Visitor() {}
    virtual void Visit(Node* n) = 0;
  };
  void Visit(Visitor* visitor);

  Matrix4 GetTransform();

  Node* parent() { return parent_; }

 private:

  void ComputeTransform();
  void SetParent(Node* n);

  Vector3 position_;
  Quaternion orientation_;

  Matrix4 transform_;
  bool transform_dirty_;

  List<Node*> children_;
  Node* parent_;

  List<Drawable*> drawables_;
};

}  // namespace gnat

#endif  // gnat_gfx_node_H
