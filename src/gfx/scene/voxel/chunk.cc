#include "gnat.h"

#include "gfx/scene/voxel/chunk.h"

namespace gnat {

template <ChunkType Type>
Chunk<Type>::Chunk()
  : changes_(new List<std::pair<Coords, VoxelType> >()),
    changes_applied_(new List<std::pair<Coords, VoxelType> >()) {
  memset(voxels_, 0, sizeof(voxels_));
  memset(neighbors_, 0, sizeof(neighbors_));
}

template <ChunkType Type>
Chunk<Type>::~Chunk() {
  delete changes_;
  delete changes_applied_;
}

template <ChunkType Type>
void Chunk<Type>::SetVoxel(Coords position, VoxelType type) {
  {
    boost::mutex::scoped_lock lock(change_lock_);
    changes_->push_back(std::make_pair(position, type));
  }
  // TODO: notify
}

template <ChunkType Type>
typename Chunk<Type>::Voxel Chunk<Type>::GetVoxel(Coords c) {
  boost::shared_lock<boost::shared_mutex> lock(voxel_lock_);
  return *get_voxel(c, this);
}

template <ChunkType Type>
void Chunk<Type>::ApplyChanges() {
  {
    boost::mutex::scoped_lock lock(change_lock_);
    std::swap(changes_, changes_applied_);
  }

  // First apply neighbor changes as necessary (also removes no-op changes).
  for (typename List<std::pair<Coords, VoxelType> >::iterator it =
       changes_applied_->begin(); it != changes_applied_->end(); ++it) {
    VoxelType t = voxels_[get_voxel_index(it->first)].type;
    if (t == it->second) {
      changes_applied_->erase(it); // no-op
    } else if (Traits::is_transparent(t) != Traits::is_transparent(it->second)) {
      Coords c = it->first;
      // Update neighbors
      for (int i = 0; i < 6; ++i) {
        Voxel* v = get_voxel(c + Traits::NEIGHBOR_COORDS[i], this);
        if (!v) {
          continue;
        } else if (Traits::is_transparent(it->second)) {
          v->neighbors &= ~Traits::NEIGHBOR_BITS_OPPOSITE[i];
        } else {
          v->neighbors |= Traits::NEIGHBOR_BITS_OPPOSITE[i];
        }
      }
    }
  }

  // Grab lock and actually apply changes (we intentionally limit time spent
  // holding the lock).
  {
    boost::upgrade_lock<boost::shared_mutex> lock(voxel_lock_); 
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    for (typename List<std::pair<Coords, VoxelType> >::iterator it =
         changes_applied_->begin(); it != changes_applied_->end(); ++it) {
      voxels_[get_voxel_index(it->first)].type = it->second;
    }
  }
}

template <ChunkType Type>
void Chunk<Type>::GenerateMeshData() {

}

template <ChunkType Type>
void Chunk<Type>::UpdateMesh() {

}

template class Chunk<TERRAIN>;
template class Chunk<STANDARD>;

}  // namespace gnat
