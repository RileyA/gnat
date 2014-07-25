#include "gfx/scene/voxel/chunk_traits.h"

namespace gnat {

const unsigned char ChunkTraitsBase::NEIGHBOR_BITS[6] = {1, 2, 4, 8, 16, 32};
const unsigned char ChunkTraitsBase::NEIGHBOR_BITS_OPPOSITE[6] = 
    {2, 1, 8, 4, 32, 16};
const float ChunkTraitsBase::VOXEL_SCALE = 1.f;

const ChunkTraits<TERRAIN>::Coords ChunkTraits<TERRAIN>::NEIGHBOR_COORDS[6] = {
  ChunkTraits<TERRAIN>::Coords(-1, 0, 0),
  ChunkTraits<TERRAIN>::Coords( 1, 0, 0),
  ChunkTraits<TERRAIN>::Coords( 0,-1, 0),
  ChunkTraits<TERRAIN>::Coords( 0, 1, 0),
  ChunkTraits<TERRAIN>::Coords( 0, 0,-1),
  ChunkTraits<TERRAIN>::Coords( 0, 0, 1)
};

const ChunkTraits<STANDARD>::Coords
    ChunkTraits<STANDARD>::NEIGHBOR_COORDS[6] = {
  ChunkTraits<STANDARD>::Coords(-1, 0, 0),
  ChunkTraits<STANDARD>::Coords( 1, 0, 0),
  ChunkTraits<STANDARD>::Coords( 0,-1, 0),
  ChunkTraits<STANDARD>::Coords( 0, 1, 0),
  ChunkTraits<STANDARD>::Coords( 0, 0,-1),
  ChunkTraits<STANDARD>::Coords( 0, 0, 1)
};

}
