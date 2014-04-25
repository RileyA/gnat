#ifndef gnat_gfx_util_osyer_mesh_H
#define gnat_gfx_util_osyer_mesh_H

#include "gnat.h"
#include "gfx/material/material.h"
#include "gfx/scene/mesh.h"

namespace Oyster {
  class Oyster;
  class Batch;
}

namespace gnat {

class OysterMesh : public Mesh {
 public:
  
  OysterMesh(Oyster::Oyster* oyster, String atlas, String batch);
  virtual ~OysterMesh() {}

  void Update();

  virtual void Draw(Material* material);

  Oyster::Batch* batch() { return batch_; }

 private:

  MeshData data_;
  bool created_;
  Oyster::Batch* batch_;
};

}  // namespace gnat

#endif  // gnat_gfx_util_osyer_mesh_H
