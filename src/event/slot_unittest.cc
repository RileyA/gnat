#include "event/dummy_event_target.h"
#include "event/slot.h"
#include "event/signal.h"
#include "event/message.h"
#include "gtest/gtest.h"

using gnat::Slot;
using gnat::DummyEventTarget;
using gnat::Message;

namespace gnat {

class SlotTest : public ::testing::Test {
 public:
  SlotTest()
    : slot_(fastdelegate::MakeDelegate(&target_, &DummyEventTarget::call)) {}

  void Call(int idx) {
    slot_.slot_(gnat::MessageAny<int>(idx));
  }

  DummyEventTarget target_;
  Slot slot_;
};

TEST_F(SlotTest, Call) {
  Call(0);
  ASSERT_EQ(target_.GetCalls(0), 1);
}

TEST_F(SlotTest, Call_Multiple) {
  Call(0);
  Call(0);
  Call(1);
  Call(2);
  Call(2);
  Call(2);
  ASSERT_EQ(target_.GetCalls(0), 2);
  ASSERT_EQ(target_.GetCalls(1), 1);
  ASSERT_EQ(target_.GetCalls(2), 3);
  ASSERT_EQ(target_.GetTotalCalls(), 6);
}

TEST_F(SlotTest, ListenTo) {
  Signal s;
  slot_.ListenTo(&s);
  EXPECT_EQ(slot_.GetListeningCount(), 1);
}

TEST_F(SlotTest, ListenToMultiple) {
  Signal s;
  Signal s2;
  slot_.ListenTo(&s);
  slot_.ListenTo(&s2);
  EXPECT_EQ(slot_.GetListeningCount(), 2);
}

TEST_F(SlotTest, StopListeningTo) {
  Signal s;
  Signal s2;
  slot_.ListenTo(&s);
  slot_.ListenTo(&s2);
  EXPECT_EQ(slot_.GetListeningCount(), 2);
  slot_.StopListeningTo(&s);
  EXPECT_EQ(slot_.GetListeningCount(), 1);
}

TEST_F(SlotTest, StopAllListening) {
  Signal s;
  Signal s2;
  slot_.ListenTo(&s);
  slot_.ListenTo(&s2);
  EXPECT_EQ(slot_.GetListeningCount(), 2);
  slot_.StopAllListening();
  EXPECT_EQ(slot_.GetListeningCount(), 0);
}

}  // namespace gnat
