#include "gfx/util/mesh_data.h"

namespace gnat {

size_t gl_sizeof(GLenum type) {
  switch (type) {
  case GL_FLOAT:
    return sizeof(float);
  default:
    NOTREACHED();
    return 0;
  }
}

MeshData::MeshData()
  : started_(false),
    current_vertex_(0),
    num_verts_(0),
    stride_(0) {}
//---------------------------------------------------------------------------

MeshData::~MeshData() {}
//---------------------------------------------------------------------------

void MeshData::AddAttribute(String name, uint32_t size, GLenum type) {
  DCHECK(!started_);
  Attribute attr = { name, stride_, size, type };
  attributes_.push_back(attr);
  stride_ += size * gl_sizeof(type);
}
//---------------------------------------------------------------------------

void MeshData::Start(size_t num_verts, size_t num_indices) {
  DCHECK(!started_);
  data_.reserve(num_verts * stride_);
  indices_.reserve(num_indices);
  started_ = true;
}
//---------------------------------------------------------------------------

void MeshData::AppendVertices(void* buf, size_t num_verts) {
  DCHECK(started_);
  AppendData(buf, num_verts_ * stride_);
}
//---------------------------------------------------------------------------

void MeshData::FinishVertex() {
  DCHECK(started_);
  DCHECK(current_vertex_ == stride_);
  current_vertex_ = 0;
  ++num_verts_;
}
//---------------------------------------------------------------------------

void MeshData::AppendData(void* buffer, size_t size) {
  DCHECK(started_);
  size_t idx = data_.size();
  data_.resize(data_.size() + size);
  memcpy(reinterpret_cast<void*>(&data_[idx]), buffer, size);
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
  
}  // namespace gnat
