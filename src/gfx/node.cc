#include "gnat.h"

#include "gfx/node.h"

namespace gnat {

Node::Node()
  : position_(Vector3::ZERO),
    orientation_(Quaternion::IDENTITY),
    transform_dirty_(false) {
  transform_.Zero();
}

Node::~Node() {
  RemoveAllChildren();
}

void Node::AddChild(Node* node) {
  DCHECK(!HasChild(node));
  children_.insert(node);
}

bool Node::HasChild(Node* node) {
  return children_.count(node);
}

void Node::RemoveChild(Node* node) {
  Set<Node*>::iterator it = children_.find(node);
  if (it != children_.end())
    children_.erase(it);
  else
    NOTREACHED();
}

void Node::RemoveAllChildren() {
  children_.clear();
}

int Node::GetNumChildren() {
  return children_.size();
}

void Node::SetPosition(const Vector3& position) {
  position_ = position;
  transform_dirty_ = true;
}

void Node::SetOrientation(const Quaternion& orientation) {
  orientation_ = orientation;
  // TODO see if a check for equality saves us much computation?
  transform_dirty_ = true;
}

Matrix4 Node::GetTransform() {
  if (transform_dirty_)
    ComputeTransform();
  return transform_;
}

void Node::ComputeTransform() {
  // TODO
}

}  // namespace gnat
