#include "gnat.h"
#include "gfx/util/oyster_mesh.h"
#include "third_party/oyster/include/Oyster.h"
#include "third_party/oyster/include/OysterMesh.h"

namespace gnat {

OysterMesh::OysterMesh(Oyster::Oyster *oyster, String atlas, String batch)
    : created_(false) {
  batch_ = oyster->createBatch(batch, atlas);
  data_.AddAttribute("position", 3, GL_FLOAT);
  data_.AddAttribute("color", 4, GL_FLOAT);
  data_.AddAttribute("uv", 2, GL_FLOAT);
}

void OysterMesh::Update() {
  Oyster::DirtyFlags f = batch_->update();
  if (!f || batch_->getMesh().getVertexCount() == 0 ||
      batch_->getMesh().getFaceCount() == 0)
    return;
  if (f) {
    const Oyster::Mesh& m = batch_->getMesh();
    int verts = f & 7 ? m.getVertexCount() : 0;
    int indices = f & 8 ? m.getFaceCount() * 3 : 0;
    DCHECK(created_ || (verts && indices));
    data_.Start(verts, indices);
    if (verts) {
      for (int i = 0; i < verts; ++i) {
        for (int j = 0; j < 3; ++j)
          data_.Append<float>(m.getVertices()[i * 3 + j]);
        for (int j = 0; j < 4; ++j)
          data_.Append<float>(m.getColors()[i * 4 + j]);
        data_.Append<float>(m.getTexcoords()[i * 2 + 0]);
        data_.Append<float>(1.0 - m.getTexcoords()[i * 2 + 1]);
        data_.FinishVertex();
      }
    }
    if (indices) {
      memcpy(&data_.indices()[0], m.getIndices(), indices * sizeof(uint32_t));
    }
    if (!created_) {
      CreateFromMeshData(&data_);
      created_ = true;
    } else {
      UpdateFromMeshData(&data_, verts, indices);
    }
  }
}

void OysterMesh::Draw(Material* material) {
  if (created_) {
    Mesh::Draw(material);
  }
}

}
