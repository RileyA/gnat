#ifndef gnat_gfx_util_mesh_data_H
#define gnat_gfx_util_mesh_data_H

#include "gnat.h"
#include "util/color.h"

#include "glew.h"
#include "gl.h"

namespace gnat {

/** A class containing raw mesh data (usually we'll want to keep mesh
 *  data in GPU memory, so avoid using this in realtime cases). */

class MeshData {
 public:

  struct Attribute {
    String name;
    size_t offset;
    size_t size;
    GLenum type;
  };

  MeshData(bool dynamic = false);
  ~MeshData();

  // Add a new vertex attribute. Returns its offset.
  void AddAttribute(String name, uint32_t size, GLenum type);

  // Start adding data, attributes should be finalized.
  void Start(size_t num_verts, size_t num_indices);

  // Append interleaved data directly, it is assumed that it is formatted
  // correctly.
  void AppendVertices(const void* buf, size_t num_verts);

  template<typename T> void Append(T* data) {
    AppendData(data, sizeof(T));
    current_vertex_ += sizeof(T);
  }

  template<typename T> void Append(T data) {
    AppendData(&data, sizeof(T));
    current_vertex_ += sizeof(T);
  }

  void FinishVertex();

  void AppendData(const void* buffer, size_t size);

  // Get buffer.
  void* GetBuffer() { return &data_[0]; }

  void AddIndex(uint32_t index);
  void AddTriangle(uint32_t v1, uint32_t v2, uint32_t v3);

  bool dynamic() { return dynamic_; }
  size_t size() { return data_.size(); }
  size_t stride() { return stride_; }

  size_t num_verts() { return num_verts_; }
  size_t num_indices() { return indices_.size(); }

  uint32_t* indices() { return &indices_[0]; }

  Vector<Attribute>& attributes() { return attributes_; }

 private:

  bool started_;
  bool dynamic_;
  
  size_t current_vertex_;

  size_t num_verts_;
  size_t stride_;

  Vector<Attribute> attributes_;
  Vector<unsigned char> data_;

  // TODO: consider using 16-bit indices until the size exceeds 2^16 - 1.
  // TODO add field for index scheme (strip/fan/etc)
  Vector<uint32_t> indices_;

};

}  // namespace gnat

#endif  // gnat_gfx_util_mesh_data_H
