#include "gfx/scene/scene_manager.h"

#include "gfx/material/material.h"
#include "gfx/material/program.h"
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
  for (DrawGroupMap::iterator it = draws_.begin(); it != draws_.end(); ++it) {
    for (DrawMap::iterator i = it->second.begin(); i != it->second.end(); ++i) {
      i->first->Use();
      ConfigureMaterial(i->first, camera);
      for (DrawQueue::iterator j = i->second.begin(); j != i->second.end();
           ++j) {
        ConfigureDrawable(*j, camera);
        (*j)->Draw();
      }
      i->first->DoneUsing();
    }
  }

  // and clean up (TODO: maybe persist this structure and invalidate it as we go?)
  draws_.clear();
}
//---------------------------------------------------------------------------

void SceneManager::ConfigureMaterial(Material* material, Camera* camera) {
  if (material->shader_program()) {
    Program* p = material->shader_program();
    Map<Program::AutoUniformType, GLuint>& uniforms = p->GetAutoUniforms();
    for (Map<Program::AutoUniformType, GLuint>::iterator it = uniforms.begin();
         it != uniforms.end(); ++it) {
      switch(it->first) {
      case Program::PROJECTION_MATRIX:
        glUniformMatrix4fv(it->second, 1, false,
                           &camera->GetProjectionMatrix().Transpose()[0][0]);
        break;
      case Program::VIEW_MATRIX:
        glUniformMatrix4fv(it->second, 1, false,
                           &camera->GetViewMatrix().Transpose()[0][0]);
        break;
      }
    }
  }
}
//---------------------------------------------------------------------------

void SceneManager::ConfigureDrawable(Drawable* draw, Camera* camera) {
  if (draw->GetMaterial()->shader_program()) {
    Program* p = draw->GetMaterial()->shader_program();
    Map<Program::AutoUniformType, GLuint>& uniforms = p->GetAutoUniforms();
    for (Map<Program::AutoUniformType, GLuint>::iterator it = uniforms.begin();
         it != uniforms.end(); ++it) {
      switch(it->first) {
      case Program::WORLD_MATRIX:
        glUniformMatrix4fv(it->second, 1, false,
                           &draw->GetParent()->GetTransform().Transpose()[0][0]);
        break;
      case Program::MVP_MATRIX:
        glUniformMatrix4fv(
            it->second, 1, false,
            &(camera->GetProjectionMatrix() * camera->GetViewMatrix() *
              draw->GetParent()->GetTransform()).Transpose()[0][0]);
        break;
      }
    }
  }
}
//---------------------------------------------------------------------------

void SceneManager::Visit(Node* n) {
  List<Drawable*>::iterator drawables = n->GetDrawables();
  for (int i = 0; i < n->GetNumDrawables(); ++i) {
    
    Material* m = (*drawables)->GetMaterial();
    DrawMap& dm = draws_[m->draw_group()];
    
    DrawMap::iterator it = dm.find((*drawables)->GetMaterial());
    if (it != dm.end()) {
      it->second.push_back(*drawables);
    } else {
      //draws_[(*drawables)->GetMaterial()] = DrawMap();
      dm[(*drawables)->GetMaterial()].push_back(*drawables);
    }
    ++drawables;
  }
}
//---------------------------------------------------------------------------

}  // namespace gnat
