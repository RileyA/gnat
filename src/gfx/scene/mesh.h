#ifndef gnat_gfx_scene_mesh_H
#define gnat_gfx_scene_mesh_H

#include "gnat.h"

#include "gfx/scene/node.h"
#include "gfx/util/mesh_data.h"

namespace gnat {

class Mesh : public Node {
 public:

  Mesh(MeshData* mesh);
  virtual ~Mesh() {}

 private:

  void CreateFromMeshData(MeshData* d);

};

}  // namespace gnat

#endif  // gnat_gfx_scene_mesh_H
