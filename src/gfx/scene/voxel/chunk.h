#ifndef gnat_gfx_scene_voxel_chunk_H
#define gnat_gfx_scene_voxel_chunk_H

#include "gnat.h"

#include "gfx/scene/node.h"
#include "gfx/scene/voxel/chunk_traits.h"
#include "gfx/util/mesh_data.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace gnat {

class Mesh;
class MeshDrawable;

template <ChunkType Type>
class Chunk : public Node {
 public:
  typedef ChunkTraits<Type> Traits;
  typedef typename ChunkTraits<Type>::Coords Coords;
  typedef typename ChunkTraits<Type>::VoxelType VoxelType;
  typedef typename ChunkTraits<Type>::VoxelLight VoxelLight;

  struct Voxel {
    VoxelType type;          // type of block
    //VoxelLight light;        // light level
    unsigned char neighbors; // Info about neighbors, encoded in first 6 bits
                             // 7th bit is 0 if invalid, 1 if valid. This is
                             // purely for optimization purposes (so we can run
                             // through the array and decide which faces to make
                             // without having to muck up the cache by looking
                             // at all of our neighbors every single time we
                             // generate a mesh).
  };

  Chunk();
  virtual ~Chunk();

  // Set a voxel. This is not an instantaneous operation, the background thread
  // will need to apply this, regenerate the mesh, and then let the main thread
  // upload to the GPU.
  void SetVoxel(Coords position, VoxelType type);

  Voxel GetVoxel(Coords c);

  // Apply chunk changes to voxel data.
  void ApplyChanges();

  // Generate mesh data.
  void GenerateMeshData();

  // Apply mesh data to GPU. Should be called from main thread only (the one on
  // which the OpenGL context is created and managed).
  void UpdateMesh();

  MeshDrawable* drawable() { return drawable_; }

  // For testing.
  bool NeighborsValid();

 private:

  // For access from the chunk updater thread only.
  Chunk<Type>* neighbors_[6];

  // How many faces we need to build a mesh.
  size_t num_faces_;

  // For access from the main thread only.
  MeshDrawable* drawable_;
  Mesh* mesh_;

  ///////////////////////////////////////////////////////////////////////////

  // Protects voxel data. Multi-reader, single writer.
  boost::shared_mutex voxel_lock_;
  Voxel voxels_[Traits::SIZE_X * Traits::SIZE_Y * Traits::SIZE_Z];

  // Protects mesh data.
  boost::mutex mesh_lock_;
  MeshData* mesh_data_;

  // Protects change data.
  boost::mutex change_lock_;
  Map<Coords, VoxelType>* changes_;
  Map<Coords, VoxelType>* changes_applied_;

  ///////////////////////////////////////////////////////////////////////////

  static size_t get_voxel_index(Coords c) {
    return c.x + c.y * Traits::SIZE_X + c.z * Traits::SIZE_X * Traits::SIZE_Y;
  }

  static Voxel* get_voxel(Coords c, Chunk* chunk) {
    if (!chunk)
      return NULL;
    if (c.x < 0)
      return get_voxel(c + Coords(Traits::SIZE_X, 0, 0), chunk->neighbors_[1]);
    else if (c.x >= Traits::SIZE_X)
      return get_voxel(c - Coords(Traits::SIZE_X, 0, 0), chunk->neighbors_[0]);
    else if (c.y < 0)
      return get_voxel(c + Coords(0, Traits::SIZE_Y, 0), chunk->neighbors_[3]);
    else if (c.y >= Traits::SIZE_Y)
      return get_voxel(c - Coords(0, Traits::SIZE_Y, 0), chunk->neighbors_[2]);
    else if (c.z < 0)
      return get_voxel(c + Coords(0, 0, Traits::SIZE_Z), chunk->neighbors_[5]);
    else if (c.z >= Traits::SIZE_Z)
      return get_voxel(c - Coords(0, 0, Traits::SIZE_Z), chunk->neighbors_[4]);
    return chunk->voxels_ + get_voxel_index(c);
  }

  static Voxel* get_voxel(Coords c, Chunk** chunk) {
    if (!(*chunk))
      return NULL;
    if (c.x < 0) {
      *chunk = (*chunk)->neighbors_[1];
      return get_voxel(c + Coords(Traits::SIZE_X, 0, 0), chunk);
    } else if (c.x >= Traits::SIZE_X) {
      *chunk = (*chunk)->neighbors_[0];
      return get_voxel(c - Coords(Traits::SIZE_X, 0, 0), chunk);
    } else if (c.y < 0) {
      *chunk = (*chunk)->neighbors_[3];
      return get_voxel(c + Coords(0, Traits::SIZE_Y, 0), chunk);
    } else if (c.y >= Traits::SIZE_Y) { 
      *chunk = (*chunk)->neighbors_[2];
      return get_voxel(c - Coords(0, Traits::SIZE_Y, 0), chunk);
    } else if (c.z < 0) {
      *chunk = (*chunk)->neighbors_[5];
      return get_voxel(c + Coords(0, 0, Traits::SIZE_Z), chunk);
    } else if (c.z >= Traits::SIZE_Z) {
      *chunk = (*chunk)->neighbors_[4];
      return get_voxel(c - Coords(0, 0, Traits::SIZE_Z), chunk);
    }
    return (*chunk)->voxels_ + get_voxel_index(c);
  }
};

}  // namespace gnat

#endif  // gnat_gfx_scene_voxel_chunk_H
