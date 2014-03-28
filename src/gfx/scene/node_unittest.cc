#include "gfx/scene/node.h"

#include "gtest/gtest.h"

namespace gnat {

const Vector3 kPosition(1.0, 2.0, 3.0);
const Quaternion kOrientation(0.5, 1.0, 0.0, 1.0);

// The trivialest of the trivial, but hey why not?
TEST(NodeTest, SetPosition) {
  Node n;
  n.SetPosition(kPosition);
  EXPECT_EQ(kPosition, n.GetPosition());
}

TEST(NodeTest, SetOrientation) {
  Node n;
  n.SetOrientation(kOrientation);
  EXPECT_EQ(kOrientation, n.GetOrientation());
}

TEST(NodeTest, UpdateTransform_Position) {
  Node n;
  Matrix4 m = Matrix4::IDENTITY;
  EXPECT_EQ(n.GetTransform(), m);
  n.SetPosition(kPosition);
  m.SetTrans(kPosition);
  EXPECT_EQ(n.GetTransform(), m);
}

TEST(NodeTest, UpdateTransform_Orientation) {
  Node n;
  Matrix4 m = Matrix4::IDENTITY;
  EXPECT_EQ(n.GetTransform(), m);
  n.SetOrientation(kOrientation);
  m = Matrix4(kOrientation);
  EXPECT_EQ(n.GetTransform(), m);
}

TEST(NodeTest, UpdateTransform_Position_Orientation) {
  Node n;
  Matrix4 m = Matrix4::IDENTITY;
  EXPECT_EQ(n.GetTransform(), m);
  n.SetPosition(kPosition);
  n.SetOrientation(kOrientation);
  m = Matrix4(kOrientation);
  m.SetTrans(kPosition);
  EXPECT_EQ(n.GetTransform(), m);
}

TEST(NodeTest, AddChild) {
  Node n;
  Node c;
  n.AddChild(&c);
  EXPECT_EQ(n.GetNumChildren(), 1);
  EXPECT_TRUE(n.HasChild(&c));
  EXPECT_TRUE(c.parent() == &n);
}

TEST(NodeTest, AddChild_Multiple) {
  Node n;
  Node c1, c2, c3;
  n.AddChild(&c1);
  n.AddChild(&c2);
  n.AddChild(&c3);
  EXPECT_EQ(n.GetNumChildren(), 3);
  EXPECT_TRUE(n.HasChild(&c1));
  EXPECT_TRUE(n.HasChild(&c2));
  EXPECT_TRUE(n.HasChild(&c3));
  EXPECT_TRUE(c1.parent() == &n);
  EXPECT_TRUE(c2.parent() == &n);
  EXPECT_TRUE(c3.parent() == &n);
}

TEST(NodeTest, RemoveChild) {
  Node n;
  Node c1, c2, c3;
  n.AddChild(&c1);
  n.AddChild(&c2);
  n.AddChild(&c3);
  EXPECT_EQ(n.GetNumChildren(), 3);
  EXPECT_TRUE(n.HasChild(&c1));
  EXPECT_TRUE(n.HasChild(&c2));
  EXPECT_TRUE(n.HasChild(&c3));
  EXPECT_EQ(c1.parent(), &n);
  n.RemoveChild(&c1);
  EXPECT_EQ(c1.parent(), (Node*)NULL);
  EXPECT_EQ(n.GetNumChildren(), 2);
  EXPECT_FALSE(n.HasChild(&c1));
  EXPECT_TRUE(n.HasChild(&c2));
  EXPECT_TRUE(n.HasChild(&c3));
}

TEST(NodeTest, RemoveChild_Multiple) {
  Node n;
  Node c1, c2, c3;
  n.AddChild(&c1);
  n.AddChild(&c2);
  n.AddChild(&c3);
  EXPECT_EQ(n.GetNumChildren(), 3);
  EXPECT_TRUE(n.HasChild(&c1));
  EXPECT_TRUE(n.HasChild(&c2));
  EXPECT_TRUE(n.HasChild(&c3));
  n.RemoveChild(&c2);
  EXPECT_EQ(n.GetNumChildren(), 2);
  EXPECT_TRUE(n.HasChild(&c1));
  EXPECT_FALSE(n.HasChild(&c2));
  EXPECT_TRUE(n.HasChild(&c3));
  n.RemoveChild(&c3);
  EXPECT_EQ(n.GetNumChildren(), 1);
  EXPECT_TRUE(n.HasChild(&c1));
  EXPECT_FALSE(n.HasChild(&c2));
  EXPECT_FALSE(n.HasChild(&c3));
  n.RemoveChild(&c1);
  EXPECT_EQ(n.GetNumChildren(), 0);
  EXPECT_FALSE(n.HasChild(&c1));
  EXPECT_FALSE(n.HasChild(&c2));
  EXPECT_FALSE(n.HasChild(&c3));
}

TEST(NodeTest, RemoveAllChildren) {
  Node n;
  Node c1, c2, c3;
  n.AddChild(&c1);
  n.AddChild(&c2);
  n.AddChild(&c3);
  EXPECT_EQ(n.GetNumChildren(), 3);
  n.RemoveAllChildren();
  EXPECT_EQ(n.GetNumChildren(), 0);
  EXPECT_FALSE(n.HasChild(&c1));
  EXPECT_FALSE(n.HasChild(&c2));
  EXPECT_FALSE(n.HasChild(&c3));
  EXPECT_TRUE(c1.parent() == NULL);
  EXPECT_TRUE(c2.parent() == NULL);
  EXPECT_TRUE(c3.parent() == NULL);
}

TEST(NodeTest, Destructed_Parent) {
  Node c1, c2, c3;
  {
    Node n;
    n.AddChild(&c1);
    n.AddChild(&c2);
    n.AddChild(&c3);
    EXPECT_EQ(n.GetNumChildren(), 3);
  }
  EXPECT_TRUE(c1.parent() == NULL);
  EXPECT_TRUE(c2.parent() == NULL);
  EXPECT_TRUE(c3.parent() == NULL);
}

TEST(NodeTest, Destructed_Child) {
  Node n;
  Node c1, c2;
  n.AddChild(&c1);
  n.AddChild(&c2);
  EXPECT_EQ(n.GetNumChildren(), 2);
  {
    Node c3;
    n.AddChild(&c3);
    EXPECT_EQ(n.GetNumChildren(), 3);
  }
  EXPECT_EQ(n.GetNumChildren(), 2);
  EXPECT_TRUE(n.HasChild(&c1));
  EXPECT_TRUE(n.HasChild(&c2));
}

TEST(NodeTest, Destructed_ParentAndChild) {
  Node c1, c2;
  {
    Node n, c3;
    n.AddChild(&c1);
    n.AddChild(&c2);
    n.AddChild(&c3);
    EXPECT_EQ(n.GetNumChildren(), 3);
  }
  EXPECT_TRUE(c1.parent() == NULL);
  EXPECT_TRUE(c2.parent() == NULL);
}

TEST(NodeTest, Destructed_ParentThenChild) {
  Node c1, c2;
  {
    Node n;
    n.AddChild(&c1);
    n.AddChild(&c2);
    {
      Node c3;
      n.AddChild(&c3);
      EXPECT_EQ(n.GetNumChildren(), 3);
    }
    EXPECT_EQ(n.GetNumChildren(), 2);
  }
}

}  // namespace gnat
