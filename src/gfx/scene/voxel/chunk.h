#ifndef gnat_gfx_scene_voxel_chunk_H
#define gnat_gfx_scene_voxel_chunk_H

#include "gnat.h"

#include "gfx/scene/node.h"
#include "gfx/scene/voxel/chunk_traits.h"
#include "gfx/util/mesh_data.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace gnat {

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

  // Apply mesh data to GPU.
  void UpdateMesh();

 private:

  // Protects mesh data.
  boost::mutex lock_;

  // Mesh data.
  MeshData mesh_data_;

  // Protects voxel data. Multi-reader, single writer.
  boost::shared_mutex voxel_lock_;

  // Protects change data.
  boost::mutex change_lock_;

  // List of mesh changes to be applied.
  List<std::pair<Coords, VoxelType> >* changes_;
  
  // List of mesh changes that have been applied.
  List<std::pair<Coords, VoxelType> >* changes_applied_;

  static size_t get_voxel_index(Coords c) {
    return c.x + c.y * Traits::SIZE_X + c.z * Traits::SIZE_X * Traits::SIZE_Y;
  }

  static Voxel* get_voxel(Coords c, Chunk* chunk) {
    if (c.x < 0)
      return get_voxel(c + Coords(Traits::SIZE_X, 0, 0), chunk);
    else if (c.x > Traits::SIZE_X)
      return get_voxel(c - Coords(Traits::SIZE_X, 0, 0), chunk);
    else if (c.y < 0)
      return get_voxel(c + Coords(0, Traits::SIZE_Y, 0), chunk);
    else if (c.y > Traits::SIZE_Y)
      return get_voxel(c - Coords(0, Traits::SIZE_Y, 0), chunk);
    else if (c.z < 0)
      return get_voxel(c + Coords(0, 0, Traits::SIZE_Z), chunk);
    else if (c.z > Traits::SIZE_Z)
      return get_voxel(c - Coords(0, 0, Traits::SIZE_Z), chunk);
    return chunk->voxels_ + get_voxel_index(c);
  }

  Chunk<Type>* neighbors_[6];

  Voxel voxels_[Traits::SIZE_X * Traits::SIZE_Y * Traits::SIZE_Z];
};

}  // namespace gnat

#endif  // gnat_gfx_scene_voxel_chunk_H
