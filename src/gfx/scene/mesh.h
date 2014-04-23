#ifndef gnat_gfx_scene_mesh_H
#define gnat_gfx_scene_mesh_H

#include "gnat.h"

#include "gfx/scene/drawable.h"
#include "gfx/util/mesh_data.h"
#include "gfx/util/buffer_object.h"

namespace gnat {

class Material;

class Mesh {
 public:

  Mesh(MeshData* mesh);
  virtual ~Mesh() {}

  void Draw(Material* material);

 private:

  void CreateFromMeshData(MeshData* d);

  VertexBufferObject vbo_;
  IndexBufferObject ibo_;

  Map<String, MeshData::Attribute> attributes_;
  uint32_t stride_;

  uint32_t num_indices_;
  uint32_t num_verts_;
};

}  // namespace gnat

#endif  // gnat_gfx_scene_mesh_H
