#include "gnat.h"
#include "gfx/scene/drawable.h"
#include "gfx/material/material.h"
#include "gfx/scene/node.h"

namespace gnat {

Drawable::Drawable() : parent_(NULL), material_(NULL) {}
//---------------------------------------------------------------------------

Drawable::~Drawable() {
  if (parent_)
    parent_->RemoveDrawable(this);
}
//---------------------------------------------------------------------------

void Drawable::SetMaterial(Material* material) {
  material_ = material;
}
//---------------------------------------------------------------------------

Material* Drawable::GetMaterial() {
  return material_;
}
//---------------------------------------------------------------------------

Node* Drawable::GetParent() {
  return parent_;
}
//---------------------------------------------------------------------------

void Drawable::SetParent(Node* parent) {
  parent_ = parent;
}
//---------------------------------------------------------------------------

}  // namespace gnat
