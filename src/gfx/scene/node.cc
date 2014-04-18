#include "gnat.h"

#include "gfx/scene/drawable.h"
#include "gfx/scene/node.h"

namespace gnat {

Node::Node()
  : position_(Vector3::ZERO),
    orientation_(Quaternion::IDENTITY),
    transform_(Matrix4::IDENTITY),
    transform_dirty_(false),
    parent_(NULL) {}
//---------------------------------------------------------------------------

Node::~Node() {
  RemoveAllChildren();
  if (parent_)
    parent_->RemoveChild(this);
}
//---------------------------------------------------------------------------

void Node::AddChild(Node* node) {
  DCHECK(!HasChild(node));
  DCHECK(!node->parent_);

#ifdef DEBUG
  // Ensure we aren't creating a cycle.
  Node* p = this;
  while (p = p->parent_)
    DCHECK(p != node);
#endif

  children_.push_back(node);
  node->parent_ = this;
}
//---------------------------------------------------------------------------

bool Node::HasChild(Node* node) {
  for (List<Node*>::iterator it = children_.begin(); it != children_.end();
       ++it) {
    if (*it == node)
      return true;
  }
  return false;
}
//---------------------------------------------------------------------------

void Node::RemoveChild(Node* node) {
  for (List<Node*>::iterator it = children_.begin(); it != children_.end();
       ++it) {
    if (*it == node) {
      (*it)->parent_ = NULL;
      children_.erase(it);
      return;
    }
  }
  NOTREACHED();
}
//---------------------------------------------------------------------------

void Node::RemoveAllChildren() {
  for (List<Node*>::iterator it = children_.begin(); it != children_.end();
       ++it) {
    (*it)->parent_ = NULL;
  }
  children_.clear();
}
//---------------------------------------------------------------------------

void Node::AddDrawable(Drawable* drawable) {
  drawables_.push_back(drawable);
  drawable->SetParent(this);
}
//---------------------------------------------------------------------------

void Node::RemoveDrawable(Drawable* drawable) {
  for (List<Drawable *>::iterator it = drawables_.begin();
       it != drawables_.end(); ++it) {
    (*it)->SetParent(NULL);
    drawables_.erase(it);
  }
  drawables_.clear();
}
//---------------------------------------------------------------------------

void Node::RemoveAllDrawables() {
  for (List<Drawable *>::iterator it = drawables_.begin();
       it != drawables_.end(); ++it) {
    (*it)->SetParent(NULL);
  }
  drawables_.clear();
}
//---------------------------------------------------------------------------

int Node::GetNumDrawables() {
  return drawables_.size();
}
//---------------------------------------------------------------------------

int Node::GetNumChildren() {
  return children_.size();
}
//---------------------------------------------------------------------------

void Node::SetPosition(const Vector3& position) {
  position_ = position;
  transform_dirty_ = true;
}
//---------------------------------------------------------------------------

void Node::SetOrientation(const Quaternion& orientation) {
  orientation_ = orientation;
  // TODO see if a check for equality saves us much computation?
  transform_dirty_ = true;
}
//---------------------------------------------------------------------------

Matrix4 Node::GetTransform() {
  if (transform_dirty_)
    ComputeTransform();
  return transform_;
}
//---------------------------------------------------------------------------

void Node::ComputeTransform() {
  transform_ = Matrix4(orientation_);
  transform_.SetTrans(position_);
  if (parent_)
    transform_ = parent_->GetTransform() * transform_;
  transform_dirty_ = false;
}
//---------------------------------------------------------------------------

}  // namespace gnat
