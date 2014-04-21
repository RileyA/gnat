#ifndef gnat_gfx_scene_mesh_H
#define gnat_gfx_scene_mesh_H

#include "gnat.h"

#include "gfx/scene/drawable.h"
#include "gfx/util/mesh_data.h"
#include "gfx/util/buffer_object.h"

namespace gnat {

class Mesh {
 public:

  Mesh(MeshData* mesh);
  virtual ~Mesh() {}

  void Draw();

 private:

  void CreateFromMeshData(MeshData* d);

  VertexBufferObject vbo_;
  IndexBufferObject ibo_;

  int num_verts_;
  int num_indices_;
  bool normals_;
  bool diffuse_;
  uint32_t stride_;

};

}  // namespace gnat

#endif  // gnat_gfx_scene_mesh_H
