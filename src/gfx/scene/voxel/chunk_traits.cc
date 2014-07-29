#include "gfx/scene/voxel/chunk_traits.h"

namespace gnat {

const unsigned char ChunkTraitsBase::NEIGHBOR_BITS[6] = {1, 2, 4, 8, 16, 32};
const unsigned char ChunkTraitsBase::NEIGHBOR_BITS_OPPOSITE[6] = 
    {2, 1, 8, 4, 32, 16};
const unsigned char ChunkTraitsBase::EMPTY_NEIGHBORS[64] = {
  6, 5, 5, 4, 5, 4, 4, 3,
  5, 4, 4, 3, 4, 3, 3, 2,
  5, 4, 4, 3, 4, 3, 3, 2,
  4, 3, 3, 2, 3, 2, 2, 1,
  5, 4, 4, 3, 4, 3, 3, 2,
  4, 3, 3, 2, 3, 2, 2, 1,
  4, 3, 3, 2, 3, 2, 2, 1,
  3, 2, 2, 1, 2, 1, 1, 0,
};
const float ChunkTraitsBase::VOXEL_SCALE = 1.f;

const float ChunkTraitsBase::VERTEX_POSITIONS[6][4][3] =
{
  { // -X
    {-0.5f,  0.5f, -0.5f},
    {-0.5f,  0.5f,  0.5f},
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f},
  },
  { // +X
    {0.5f,  0.5f,  0.5f},
    {0.5f,  0.5f, -0.5f},
    {0.5f, -0.5f,  0.5f},
    {0.5f, -0.5f, -0.5f},
  },
  { // -Y
    {-0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f,  0.5f},
    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
  },
  { // +Y
    {-0.5f, 0.5f, -0.5f},
    { 0.5f, 0.5f, -0.5f},
    {-0.5f, 0.5f,  0.5f},
    { 0.5f, 0.5f,  0.5f},
  },
  { // -Z
    { 0.5f, 0.5f, -0.5f},
    {-0.5f, 0.5f, -0.5f},
    { 0.5f,-0.5f, -0.5f},
    {-0.5f,-0.5f, -0.5f},
  },
  { // +Z
    {-0.5f, 0.5f, 0.5f},
    { 0.5f, 0.5f, 0.5f},
    {-0.5f,-0.5f, 0.5f},
    { 0.5f,-0.5f, 0.5f},
  },
};

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
