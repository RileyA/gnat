#include "event/dummy_event_target.h"
#include "event/observer.h"
#include "event/slot.h"
#include "event/signal.h"
#include "event/message.h"
#include "gtest/gtest.h"

using gnat::Slot;
using gnat::DummyEventTarget;
using gnat::Observer;

namespace { 

class ObserverTest : public ::testing::Test {
 public:
  ObserverTest() {}

  Observer observer_;
  DummyEventTarget dummy_;
};

TEST_F(ObserverTest, GetNonexistentSlot) {
  EXPECT_EQ(NULL, observer_.GetSlot("derp"));
}

TEST_F(ObserverTest, CreateSlot_Get) {
  Slot* s = observer_.CreateSlot("test",
    fastdelegate::MakeDelegate(&dummy_, &DummyEventTarget::call));
  EXPECT_NE(s, (Slot*)NULL);
  EXPECT_EQ(s, observer_.GetSlot("test"));
}

TEST_F(ObserverTest, CreateMultipleSlots) {
  Slot* s1 = observer_.CreateSlot("test1",
    fastdelegate::MakeDelegate(&dummy_, &DummyEventTarget::call));
  Slot* s2 = observer_.CreateSlot("test2",
    fastdelegate::MakeDelegate(&dummy_, &DummyEventTarget::call));
  EXPECT_EQ(s1, observer_.GetSlot("test1"));
  EXPECT_EQ(s2, observer_.GetSlot("test2"));
  EXPECT_EQ(NULL, observer_.GetSlot("test"));
}

}

