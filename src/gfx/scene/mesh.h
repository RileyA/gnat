#ifndef gnat_gfx_scene_mesh_H
#define gnat_gfx_scene_mesh_H

#include "gnat.h"

#include "gfx/material/material.h"
#include "gfx/scene/drawable.h"
#include "gfx/util/mesh_data.h"
#include "gfx/util/buffer_object.h"

namespace gnat {

class Mesh {
 public:

  Mesh(MeshData* mesh);
  virtual ~Mesh() {}

  virtual void Draw(Material* material);

  static Mesh* Load(String filename);

  void UpdateFromMeshData(MeshData *data, bool verts = true,
                          bool indices = true);

 protected:

  Mesh() {}
  void CreateFromMeshData(MeshData* d);

 private:

  static Mesh* LoadPLY(String filename);

  VertexBufferObject vbo_;
  IndexBufferObject ibo_;

  Map<String, MeshData::Attribute> attributes_;
  uint32_t stride_;

  uint32_t num_indices_;
  uint32_t num_verts_;

  uint32_t allocated_verts_;
  uint32_t allocated_indices_;
};

}  // namespace gnat

#endif  // gnat_gfx_scene_mesh_H
