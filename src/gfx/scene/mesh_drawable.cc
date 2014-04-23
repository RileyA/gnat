#include "gfx/scene/mesh_drawable.h"
#include "gfx/scene/mesh.h"

namespace gnat {

MeshDrawable::MeshDrawable(Mesh* mesh) :mesh_(mesh) {}
//---------------------------------------------------------------------------

void MeshDrawable::Draw() {
  mesh_->Draw(GetMaterial());
}
//---------------------------------------------------------------------------

}  // namespace gnat
