#include "gnat.h"

#include "gfx/scene/voxel/chunk.h"
#include "gfx/scene/mesh.h"
#include "gfx/scene/mesh_drawable.h"
#include "gfx/util/mesh_data.h"

namespace gnat {

template <ChunkType Type>
Chunk<Type>::Chunk()
  : mesh_data_(0),
    num_faces_(0),
    mesh_(0),
    changes_(new Map<Coords, VoxelType>()),
    changes_applied_(new Map<Coords, VoxelType>()) {
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
    std::lock_guard<std::mutex> lock(change_lock_);
    (*changes_)[position] = type;
  }
  // TODO: notify
}

template <ChunkType Type>
typename Chunk<Type>::Voxel Chunk<Type>::GetVoxel(Coords c) {
  std::lock_guard<std::mutex> lock(voxel_lock_);
  return *get_voxel(c, this);
}

template <ChunkType Type>
void Chunk<Type>::ApplyChanges() {
  {
    std::lock_guard<std::mutex> lock(change_lock_);
    std::swap(changes_, changes_applied_);
  }

  // First apply neighbor changes as necessary (also removes no-op changes).
  for (typename Map<Coords, VoxelType>::iterator it =
       changes_applied_->begin(); it != changes_applied_->end(); ++it) {

    Voxel* current = &voxels_[get_voxel_index(it->first)];
    VoxelType t = current->type;

    if (t == it->second) {
      continue; // no-op TODO remove
    } else if (Traits::is_transparent(t) != Traits::is_transparent(it->second)) {
      Coords c = it->first;
      // Update neighbors
      for (int i = 0; i < 6; ++i) {
        Chunk<Type>* n = this;
        Voxel* v = get_voxel(c + Traits::NEIGHBOR_COORDS[i], &n);
        if (!v) {
          if (Traits::is_transparent(it->second) && !Traits::is_transparent(t)) {
            --num_faces_;
          } else if(!Traits::is_transparent(it->second) && Traits::is_transparent(t)) {
            ++num_faces_;
          }
          continue;
        }

        if (Traits::is_transparent(it->second)) {
          // If our neighbor is solid and we weren't but now are, they will
          // need an additional face.
          if (current->neighbors & Traits::NEIGHBOR_BITS[i] &&
              !Traits::is_transparent(t)) {
            ++n->num_faces_;
          }
          v->neighbors &= ~Traits::NEIGHBOR_BITS_OPPOSITE[i];
        } else {
          // We are solid, neighbor is too
          if (current->neighbors & Traits::NEIGHBOR_BITS[i] && n) {
            --n->num_faces_; // their block loses a face
          } else { // we are solid neighbor is nothing
            ++num_faces_; // our block gains a face
          }
          v->neighbors |= Traits::NEIGHBOR_BITS_OPPOSITE[i];
        }
      }
    }
  }

  // Grab lock and actually apply changes (we intentionally limit time spent
  // holding the lock).
  {
    std::lock_guard<std::mutex> lock(voxel_lock_); 

    for (typename Map<Coords, VoxelType>::iterator it =
         changes_applied_->begin(); it != changes_applied_->end(); ++it) {
      voxels_[get_voxel_index(it->first)].type = it->second;
    }
  }
}

template <ChunkType Type>
void Chunk<Type>::GenerateMeshData() {
  MeshData* data = new MeshData();

  // Generate some thangs
  data->AddAttribute("position", 3, GL_FLOAT);
  data->AddAttribute("uv", 2, GL_FLOAT);

  data->Start(num_faces_ * 4, num_faces_ * 6);

  int index = 0;
  int vertex_index = 0;
  for (int z = 0; z < 16; ++z) {
    for (int y = 0; y < 16; ++y) {
      for (int x = 0; x < 16; ++x) {
        ++index;
        if (voxels_[index].neighbors >= 63 ||
            Traits::is_nothing(voxels_[index].type)) {
          continue;
        }
        Vector3 base_pos(x + 0.5, y + 0.5, z + 0.5);
        for (int i = 0; i < 6; ++i) {
          if (!(voxels_[index].neighbors & Traits::NEIGHBOR_BITS[i])) {
            float vertices[12];
            float texcoords[4][2] = {
              {0.f, 0.f},
              {1.f, 0.f},
              {0.f, 1.f},
              {1.f, 1.f},
            };

            for (int j = 0; j < 4; ++j) {
              memcpy(vertices, Traits::VERTEX_POSITIONS[i][j], sizeof(vertices));
              for (int v = 0; v < 4; ++v) {
                vertices[v * 3 + 0] += x;
                vertices[v * 3 + 1] += y;
                vertices[v * 3 + 2] += z;
              }
              data->AppendData(vertices, sizeof(float) * 3);
              data->AppendData(texcoords[j], sizeof(float) * 2);
            }
            data->AddTriangle(vertex_index, vertex_index + 2, vertex_index + 3);
            data->AddTriangle(vertex_index, vertex_index + 3, vertex_index + 1);
            vertex_index += 4;
          }
        }
      }
    }
  }

  printf("Num verts: %d\n", vertex_index);

  {
    std::lock_guard<std::mutex> lock(mesh_lock_);
    delete mesh_data_;
    mesh_data_ = data;
  }
}

template <ChunkType Type>
void Chunk<Type>::UpdateMesh() {
  MeshData* data = 0;

  {
    std::lock_guard<std::mutex> lock(mesh_lock_);
    data = mesh_data_;
    mesh_data_ = 0;
  }

  if (!mesh_) {
    mesh_ = new Mesh(data);
    drawable_ = new MeshDrawable(mesh_);
    AddDrawable(drawable_);
  } else {
    DCHECK(drawable_);
    mesh_->UpdateFromMeshData(data, true, true);
  }

  delete data;
}

template <ChunkType Type>
bool Chunk<Type>::NeighborsValid() {
  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 16; ++j) {
      for (int k = 0; k < 16; ++k) {
        Coords c(i, j, k);
        Voxel* v = &voxels_[get_voxel_index(c)];
        for (int l = 0; l < 6; ++l) {
          Voxel* n = get_voxel(c + Traits::NEIGHBOR_COORDS[l], this);
          if (!n)
            continue;
          if (Traits::is_transparent(n->type) ==
              (v->neighbors & Traits::NEIGHBOR_BITS[l])) {
            return false;
          }
        }
      }
    }
  }
  return true;
}

template class Chunk<TERRAIN>;
template class Chunk<STANDARD>;

}  // namespace gnat
