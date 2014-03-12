#include "event/dummy_event_target.h"
#include "event/observable.h"
#include "event/slot.h"
#include "event/signal.h"
#include "event/message.h"
#include "gtest/gtest.h"

using gnat::Signal;
using gnat::DummyEventTarget;
using gnat::Observable;

namespace { 

class ObservableTest : public ::testing::Test {
 public:
  ObservableTest() {}

  Observable observable_;
  DummyEventTarget dummy_;
};

TEST_F(ObservableTest, GetNonexistentSignal) {
  EXPECT_EQ(NULL, observable_.GetSignal("derp"));
}

TEST_F(ObservableTest, CreateSlot_Get) {
  Signal* s = observable_.CreateSignal("test");
  EXPECT_NE(s, (Signal*)NULL);
  EXPECT_EQ(s, observable_.GetSignal("test"));
}

TEST_F(ObservableTest, CreateMultipleSlots) {
  Signal* s = observable_.CreateSignal("test1");
  Signal* s2 = observable_.CreateSignal("test2");
  EXPECT_NE(s, (Signal*)NULL);
  EXPECT_NE(s2, (Signal*)NULL);
  EXPECT_EQ(s, observable_.GetSignal("test1"));
  EXPECT_EQ(s2, observable_.GetSignal("test2"));
  EXPECT_EQ((Signal*)NULL, observable_.GetSignal("test"));
}

}

