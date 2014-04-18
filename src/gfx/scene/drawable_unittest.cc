#include "gfx/scene/node.h"
#include "gfx/scene/drawable.h"
#include "gfx/material/material.h"

#include "gtest/gtest.h"

namespace gnat {

class FakeDrawable : public Drawable {
  void Draw() {}
};

TEST(DrawableTest, SetMaterial) {
  Material* material = new Material("material");
  Drawable* draw = new FakeDrawable();
  EXPECT_EQ(draw->GetMaterial(), (Material*)NULL);
  draw->SetMaterial(material);
  EXPECT_EQ(draw->GetMaterial(), material);
}

TEST(DrawableTest, SetParent) {
  Node* node = new Node();
  Drawable* draw = new FakeDrawable();
  EXPECT_EQ(draw->GetParent(), (Node*)NULL);
  node->AddDrawable(draw);
  EXPECT_EQ(draw->GetParent(), node);
}

}  // namespace gnat
