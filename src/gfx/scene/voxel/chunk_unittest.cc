#include "gfx/scene/voxel/chunk_traits.h"
#include "gfx/scene/voxel/chunk.h"

#include "gtest/gtest.h"

namespace gnat {

typedef Chunk<STANDARD> StandardChunk;
typedef ChunkTraits<STANDARD>::Coords Coords;
typedef ChunkTraits<STANDARD> Traits;
  
TEST(ChunkTest, ApplyChanges) {
  StandardChunk* c = new StandardChunk();
  c->SetVoxel(Coords(8,8,8), 1);
  EXPECT_EQ(c->GetVoxel(Coords(8,8,8)).type, 0);
  c->ApplyChanges();
  EXPECT_EQ(c->GetVoxel(Coords(8,8,8)).type, 1);
}

TEST(ChunkTest, ApplyChanges2) {
  StandardChunk* c = new StandardChunk();
  c->SetVoxel(Coords(8,8,8), 1);
  c->SetVoxel(Coords(8,8,9), 1);
  EXPECT_EQ(c->GetVoxel(Coords(8,8,8)).type, 0);
  EXPECT_EQ(c->GetVoxel(Coords(8,8,9)).type, 0);
  c->ApplyChanges();
  EXPECT_EQ(c->GetVoxel(Coords(8,8,8)).type, 1);
  EXPECT_EQ(c->GetVoxel(Coords(8,8,9)).type, 1);
}

TEST(ChunkTest, ApplyChanges_Conflict) {
  StandardChunk* c = new StandardChunk();
  c->SetVoxel(Coords(8,8,8), 1);
  c->SetVoxel(Coords(8,8,8), 2);
  c->SetVoxel(Coords(8,8,8), 3);
  EXPECT_EQ(c->GetVoxel(Coords(8,8,8)).type, 0);
  c->ApplyChanges();
  EXPECT_EQ(c->GetVoxel(Coords(8,8,8)).type, 3);
}

TEST(ChunkTest, CheckNeighbors) {
  StandardChunk* c = new StandardChunk();
  c->SetVoxel(Coords(8,8,8), 1);
  c->ApplyChanges();
  EXPECT_EQ(c->GetVoxel(Coords(7, 8, 8)).neighbors,
            Traits::NEIGHBOR_BITS[1]);
  EXPECT_EQ(c->GetVoxel(Coords(9, 8, 8)).neighbors,
            Traits::NEIGHBOR_BITS[0]);
  EXPECT_EQ(c->GetVoxel(Coords(8, 7, 8)).neighbors,
            Traits::NEIGHBOR_BITS[3]);
  EXPECT_EQ(c->GetVoxel(Coords(8, 9, 8)).neighbors,
            Traits::NEIGHBOR_BITS[2]);
  EXPECT_EQ(c->GetVoxel(Coords(8, 8, 7)).neighbors,
            Traits::NEIGHBOR_BITS[5]);
  EXPECT_EQ(c->GetVoxel(Coords(8, 8, 9)).neighbors,
            Traits::NEIGHBOR_BITS[4]);
  EXPECT_EQ(c->GetVoxel(Coords(8, 8, 8)).neighbors, 0);
}

TEST(ChunkTest, CheckNeighbors2) {
  StandardChunk* c = new StandardChunk();
  c->SetVoxel(Coords(8,8,8), 1);
  c->SetVoxel(Coords(8,8,9), 1);
  c->ApplyChanges();
  EXPECT_EQ(c->GetVoxel(Coords(7, 8, 8)).neighbors,
            Traits::NEIGHBOR_BITS[1]);
  EXPECT_EQ(c->GetVoxel(Coords(9, 8, 8)).neighbors,
            Traits::NEIGHBOR_BITS[0]);
  EXPECT_EQ(c->GetVoxel(Coords(8, 7, 8)).neighbors,
            Traits::NEIGHBOR_BITS[3]);
  EXPECT_EQ(c->GetVoxel(Coords(8, 9, 8)).neighbors,
            Traits::NEIGHBOR_BITS[2]);
  EXPECT_EQ(c->GetVoxel(Coords(8, 8, 7)).neighbors,
            Traits::NEIGHBOR_BITS[5]);
  EXPECT_EQ(c->GetVoxel(Coords(8, 8, 9)).neighbors,
            Traits::NEIGHBOR_BITS[4]);
  EXPECT_EQ(c->GetVoxel(Coords(8, 8, 8)).neighbors,
            Traits::NEIGHBOR_BITS[5]);
}

}  // namespace gnat
