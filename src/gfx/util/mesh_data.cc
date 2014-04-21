#include "gfx/util/mesh_data.h"

namespace gnat {

void MeshData::AddPosition(Vector3 p) {
  positions_.push_back(p.x);
  positions_.push_back(p.y);
  positions_.push_back(p.z);
}
//---------------------------------------------------------------------------

void MeshData::AddNormal(Vector3 n) {
  normals_.push_back(n.x);
  normals_.push_back(n.y);
  normals_.push_back(n.z);
}
//---------------------------------------------------------------------------

void MeshData::AddDiffuse(ColorF32 c) {
  diffuse_.push_back(c.r);
  diffuse_.push_back(c.g);
  diffuse_.push_back(c.b);
  diffuse_.push_back(c.a);
}
//---------------------------------------------------------------------------

void MeshData::AddTexcoordSet() {
  texcoords_.push_back(Vector<float>());
}
//---------------------------------------------------------------------------

void MeshData::AddTexcoord(uint32_t index, float u, float v) {
  DCHECK(texcoords_.size() > index && index > 0);
  texcoords_[index].push_back(u);
  texcoords_[index].push_back(v);
}
//---------------------------------------------------------------------------

void MeshData::AddTexcoord(uint32_t index, float u, float v, float w) {
  texcoords_[index].push_back(u);
  texcoords_[index].push_back(v);
  texcoords_[index].push_back(w);
}
//---------------------------------------------------------------------------

void MeshData::AddIndex(uint32_t index) {
  indices_.push_back(index);
}
//---------------------------------------------------------------------------

void MeshData::AddTriangle(uint32_t v1, uint32_t v2, uint32_t v3) {
  indices_.push_back(v1);
  indices_.push_back(v2);
  indices_.push_back(v3);
}
//---------------------------------------------------------------------------
  
bool MeshData::IsValid() {
  int num_verts = positions_.size() / 3;
  if (positions_.size() % 3 != 0)
    return false;
  if (HasNormals() && normals_.size() != num_verts * 3)
    return false;
  if (HasDiffuse() && diffuse_.size() != num_verts)
    return false;
  return true;
}
//---------------------------------------------------------------------------

// Size of vertices not counting indices.
size_t MeshData::size() {
  DCHECK(IsValid());
  size_t out = 0;
  out += positions_.size() * sizeof(float);
  out += normals_.size() * sizeof(float);
  out += diffuse_.size() * sizeof(uint8_t);  // we'll round to 8bit
  return out;
}
//---------------------------------------------------------------------------

}  // namespace gnat
