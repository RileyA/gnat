#ifndef gnat_gfx_util_mesh_data_H
#define gnat_gfx_util_mesh_data_H

#include "gnat.h"
#include "gfx/math/vector3.h"
#include "util/color.h"

namespace gnat {

/** A class containing raw mesh data (usually we'll want to keep mesh
 *  data in GPU memory, so avoid using this in realtime cases). */

// TODO: Write an interleaved version of this to minimize copies.
class MeshData {
 public:

  MeshData();
  ~MeshData();

  void AddPosition(Vector3 p);
  void AddNormal(Vector3 n);
  void AddDiffuse(ColorF32 c);

  void AddTexcoordSet();
  void AddTexcoord(uint32_t index, float u, float v);
  void AddTexcoord(uint32_t index, float u, float v, float w);

  void AddIndex(uint32_t index);
  void AddTriangle(uint32_t v1, uint32_t v2, uint32_t v3);

  float* positions() { return &positions_[0]; }
  float* normals() { return &normals_[0]; }
  float* diffuse() { return &diffuse_[0]; }
  float* texcoords() { return &diffuse_[0]; }

 private:
  
  // TODO: should these be Real rather than float? GL precision is likely to
  // be a different beast than anywhere else, so for the meantime 'float' works.
  Vector<float> positions_;
  Vector<float> normals_;
  Vector<float> diffuse_;
  Vector<Vector<float> > texcoords_;

  // TODO: consider using 16-bit indices until the size exceeds 2^16 - 1.
  // TODO add field for index scheme (strip/fan/etc)
  Vector<uint32_t> indices_;

};

}  // namespace gnat

#endif  // gnat_gfx_util_mesh_data_H
