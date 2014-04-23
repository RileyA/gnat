#include "gfx/scene/mesh.h"

#include "gfx/material/material.h"
#include "gfx/material/program.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace gnat {

Mesh::Mesh(MeshData* data) {
  CreateFromMeshData(data);
}

void Mesh::CreateFromMeshData(MeshData* data) {
  vbo_ = VertexBufferObject(data->size(), GL_STATIC_DRAW);
  ibo_ =
      IndexBufferObject(data->num_indices() * sizeof(uint32_t), GL_STATIC_DRAW);
  unsigned char* buffer = static_cast<unsigned char*>(vbo_.MapBuffer());
  memcpy(buffer, data->GetBuffer(), data->size());
  vbo_.UnmapBuffer();

  Vector<MeshData::Attribute>& attrs = data->attributes();
  for (int i = 0; i < attrs.size(); ++i)
    attributes_[attrs[i].name] = attrs[i];

  stride_ = data->stride();

  buffer = static_cast<unsigned char*>(ibo_.MapBuffer());
  num_indices_ = data->num_indices();
  memcpy(buffer, data->indices(), num_indices_ * sizeof(uint32_t));
  ibo_.UnmapBuffer();
}

void Mesh::Draw(Material* material) {
  vbo_.Bind();
  ibo_.Bind();

  Program* program = material->shader_program();

  if (program) {
    Vector<std::pair<String, GLuint> >& attribs =
        program->GetVertexAttributes();
    for (int i = 0; i < attribs.size(); ++i) {
      if (attributes_.count(attribs[i].first)) {
        MeshData::Attribute attr = attributes_[attribs[i].first];
        glVertexAttribPointer(attribs[i].second, attr.size, attr.type, false,
                              stride_, BUFFER_OFFSET(attr.offset));
        glEnableVertexAttribArray(attribs[i].second);
        printf(
            "%s| idx: %d, size: %d, type: %d, norm: %d, stride: %d, offset: %p;\n",
            attr.name.c_str(), attribs[i].second, attr.size, attr.type, false, stride_,
            BUFFER_OFFSET(attr.offset));
      }
    }
  }

  glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, (GLvoid*)0);

  ibo_.Unbind();
  vbo_.Unbind();

}

}  // namespace gnat
