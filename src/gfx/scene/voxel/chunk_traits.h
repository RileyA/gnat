#ifndef gnat_gfx_scene_voxel_chunk_traits_H
#define gnat_gfx_scene_voxel_chunk_traits_H

namespace gnat {
  enum ChunkType {
    TERRAIN,  // Arranged in a 2d grid, a la Minecraft.
    STANDARD, // Arranged in a 3d grid.
  };

  struct ChunkTraitsBase {
    static const unsigned char NEIGHBOR_BITS[6];
    static const unsigned char NEIGHBOR_BITS_OPPOSITE[6];
    static const unsigned char NEIGHBOR_VALID_BIT = 64;
    static const unsigned char EMPTY_NEIGHBORS[64];
    static const float VOXEL_SCALE;
    static const float VERTEX_POSITIONS[6][4][3];
    typedef unsigned char VoxelType;
    typedef unsigned char VoxelLight;
    static bool is_transparent(VoxelType type) {
      return !type;
    }
    static bool is_nothing(VoxelType type) {
      return !type;
    }
  };

  template <ChunkType Type>
  struct ChunkTraits : public ChunkTraitsBase {};

  template <typename T, typename U, typename V>
  struct ChunkCoords {
    ChunkCoords(T x_in, U y_in, V z_in)
      : x(x_in), y(y_in), z(z_in) {}
    T x, y, z;

    ChunkCoords operator+ (const ChunkCoords<T, U, V>& c) {
      return ChunkCoords(x + c.x, y + c.y, z + c.z);
    }

    ChunkCoords operator- (const ChunkCoords<T, U, V>& c) {
      return ChunkCoords(x - c.x, y - c.y, z - c.z);
    }

    bool operator< (const ChunkCoords<T, U, V>& c) const {
      return x < c.x || (x == c.x && (y < c.y || (y == c.y && (z < c.z))));
    }
  };

  template<>
  struct ChunkTraits<TERRAIN> : public ChunkTraitsBase  {
    static const int SIZE_X = 16;
    static const int SIZE_Y = 128;
    static const int SIZE_Z = 16;
    typedef ChunkCoords<signed char, signed short, signed char> Coords;
    static const Coords NEIGHBOR_COORDS[6];
  };

  template<>
  struct ChunkTraits<STANDARD> : public ChunkTraitsBase  {
    static const int SIZE_X = 16;
    static const int SIZE_Y = 16;
    static const int SIZE_Z = 16;
    typedef ChunkCoords<signed char, signed char, signed char> Coords;
    static const Coords NEIGHBOR_COORDS[6];
  };
}

#endif  // gnat_gfx_scene_voxel_chunk_traits_H
