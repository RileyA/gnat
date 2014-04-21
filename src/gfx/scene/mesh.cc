#include "gfx/scene/mesh.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace gnat {

Mesh::Mesh(MeshData* data) {
  CreateFromMeshData(data);
}

void Mesh::CreateFromMeshData(MeshData* data) {
  
  vbo_ = VertexBufferObject(data->size(), GL_STATIC_DRAW);
  ibo_ = IndexBufferObject(data->num_indices() * sizeof(uint32_t), GL_STATIC_DRAW);

  unsigned char* buffer = static_cast<unsigned char*>(vbo_.MapBuffer());
  normals_ = data->HasNormals();
  stride_ = (3 + (normals_?3:0)) * sizeof(float);

  num_verts_ = data->num_verts();
  for (int i = 0; i < num_verts_; ++i) {
    float* fbuffer = reinterpret_cast<float*>(buffer);
    for (int j = 0; j < 3; ++j)
      fbuffer[j] = data->positions()[j];
    if (normals_)
      for (int j = 0; j < 3; ++j)
        fbuffer[j] = data->normals()[j];
    buffer += stride_;
  }

  vbo_.UnmapBuffer();

  buffer = static_cast<unsigned char*>(ibo_.MapBuffer());
  num_indices_ = data->num_indices();
  memcpy(buffer, data->indices(), num_indices_ * sizeof(uint32_t));
  ibo_.UnmapBuffer();

 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                       stride_, BUFFER_OFFSET(0));
 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride_,
                       BUFFER_OFFSET(sizeof(float)*3));
}

void Mesh::Draw() {
  vbo_.Bind();
  ibo_.Bind();

  glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, (GLvoid*)0);

  ibo_.Unbind();
  vbo_.Unbind();
}

}  // namespace gnat
