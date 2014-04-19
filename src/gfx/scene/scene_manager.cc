#include "gfx/scene/scene_manager.h"

#include "gfx/scene/camera.h"
#include "gfx/scene/drawable.h"

namespace gnat {


SceneManager::SceneManager() {

}
//---------------------------------------------------------------------------

SceneManager::~SceneManager() {

}
//---------------------------------------------------------------------------

void SceneManager::Draw(Node* root, Camera* camera) {
  // gather up all the draws
  root->Visit(this);
  
  // execute the draws
  for (DrawMap::iterator i = draws_.begin(); i != draws_.end(); ++i) {
    for (DrawQueue::iterator j = i->second.begin(); j != i->second.end(); ++j) {
      (*j)->Draw();
    }
  }

  // and clean up (TODO: maybe persist this structure and invalidate it as we go?)
  draws_.clear();
}
//---------------------------------------------------------------------------

void SceneManager::Visit(Node* n) {
  List<Drawable*>::iterator drawables = n->GetDrawables();
  for (int i = 0; i < n->GetNumDrawables(); ++i) {
    DrawMap::iterator it = draws_.find((*drawables)->GetMaterial());
    if (it != draws_.end()) {
      it->second.push_back(*drawables);
    } else {
      //draws_[(*drawables)->GetMaterial()] = DrawMap();
      draws_[(*drawables)->GetMaterial()].push_back(*drawables);
    }
    ++drawables;
  }
}
//---------------------------------------------------------------------------

}  // namespace gnat
