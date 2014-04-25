#include "gfx/scene/mesh.h"

#include "gfx/material/material.h"
#include "gfx/material/program.h"
#include "gfx/math/vector3.h"
#include "util/file_utils.h"

#include <fstream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace gnat {

Mesh::Mesh(MeshData* data) {
  CreateFromMeshData(data);
}

void Mesh::CreateFromMeshData(MeshData* data) {
  vbo_ = VertexBufferObject(data->size(),
                            data->dynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  ibo_ = IndexBufferObject(data->num_indices() * sizeof(uint32_t),
                           data->dynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  num_verts_ = allocated_verts_ = data->num_verts();
  unsigned char* buffer = static_cast<unsigned char*>(vbo_.MapBuffer());
  memcpy(buffer, data->GetBuffer(), data->size());
  vbo_.UnmapBuffer();

  Vector<MeshData::Attribute>& attrs = data->attributes();
  for (int i = 0; i < attrs.size(); ++i)
    attributes_[attrs[i].name] = attrs[i];

  stride_ = data->stride();

  buffer = static_cast<unsigned char*>(ibo_.MapBuffer());
  num_indices_ = allocated_indices_ = data->num_indices();
  memcpy(buffer, data->indices(), num_indices_ * sizeof(uint32_t));
  ibo_.UnmapBuffer();
  DCHECK(glGetError() == GL_NO_ERROR);
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
      }
    }
  }

  glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, (GLvoid*)0);

  ibo_.Unbind();
  vbo_.Unbind();
}

void Mesh::UpdateFromMeshData(MeshData* data, bool verts, bool indices) {
  if (verts) {
    if (allocated_verts_ < data->num_verts()) {
      vbo_ = VertexBufferObject(
          data->size(), data->dynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
      allocated_verts_ = data->num_verts();
    }
    num_verts_ = data->num_verts();
    unsigned char* buffer = static_cast<unsigned char*>(vbo_.MapBuffer());
    memcpy(buffer, data->GetBuffer(), data->size());
    vbo_.UnmapBuffer();
  }
  if (indices) {
    if (allocated_indices_ < data->num_indices()) {
      ibo_ =
          IndexBufferObject(data->num_indices() * sizeof(uint32_t),
                            data->dynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
      allocated_indices_ = data->num_indices();
    }
    num_indices_ = data->num_indices();
    unsigned char* buffer = static_cast<unsigned char*>(ibo_.MapBuffer());
    memcpy(buffer, data->indices(), data->num_indices());
    ibo_.UnmapBuffer();
  }
}

Mesh* Mesh::Load(String filename) {
  int idx = filename.find_last_of(".");
  String ext = filename.substr(idx + 1, String::npos);

  if (ext == "ply")
    return LoadPLY(filename);
  else
    return NULL;
}

Mesh* Mesh::LoadPLY(String filename) {
  std::ifstream file((FileUtils::GetBaseFilePath() + filename).c_str());
  if (!file.good())
    return NULL;
  std::string tmp;
  file >> tmp;
  if (tmp != "ply")
    return NULL;

  while (tmp != "element") file >> tmp;
  file >> tmp; // vertex

  int num_verts;
  file >> num_verts;

  MeshData d;

  bool positions = false;;
  bool normals = false;;
  bool uv = false;

  // advance to properties
  while (tmp != "property") file >> tmp;

  while (tmp != "element") {
    file >> tmp;
    String name;
    file >> name;
    if (name == "x") {
      d.AddAttribute("position", 3, GL_FLOAT);
      positions = true;
    } else if (name == "nx") {
      d.AddAttribute("normal", 3, GL_FLOAT);
      normals = true;
    } else if (name == "s") {
      d.AddAttribute("uv", 2, GL_FLOAT);
      uv = true;
    }
    while (tmp != "property" && tmp != "element") file >> tmp;
  }

  file >> tmp;
  int num_faces;
  file >> num_faces;

  while (tmp != "end_header") file >> tmp;

  d.Start(num_verts, num_faces * 3);

  for (int i = 0; i < num_verts; ++i) {
    if (positions) {
      Vector3 p;
      file >> p.x;
      file >> p.y;
      file >> p.z;
      d.Append<Vector3>(p);
    }
    if (normals) {
      Vector3 n;
      file >> n.x;
      file >> n.y;
      file >> n.z;
      d.Append<Vector3>(n);
    }
    if (uv) {
      float s, t;
      file >> s;
      file >> t;
      d.Append<float>(s);
      d.Append<float>(t);
    }
    d.FinishVertex();
  }

  for (int i = 0; i < num_faces; ++i) {
    uint32_t idx[3];
    uint32_t num;
    file >> num;
    if (num != 3)
      return NULL;
    file >> idx[0];
    file >> idx[1];
    file >> idx[2];
    d.AddTriangle(idx[0], idx[1], idx[2]);
  }

  return new Mesh(&d);
}

}  // namespace gnat
