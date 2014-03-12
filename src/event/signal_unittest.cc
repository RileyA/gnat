
#include "event/dummy_event_target.h"
#include "event/slot.h"
#include "event/signal.h"
#include "event/message.h"
#include "gtest/gtest.h"

using gnat::Slot;
using gnat::DummyEventTarget;
using gnat::Message;

namespace gnat {

class SignalTest : public ::testing::Test {
 public:

  SignalTest()
    :slot_(fastdelegate::MakeDelegate(&target_, &DummyEventTarget::call)) {
  }

  void AddListener() {
    signal_.AddListener(&slot_);
    EXPECT_EQ(signal_.GetNumListeners(), 1);
    EXPECT_EQ(slot_.GetListeningCount(), 1);
  }

  void FireInt(int idx) {
    MessageAny<int> m(idx);
    signal_.Fire(m);
  }

  void FireIntPtr(int idx) {
    MessageAny<int> m(idx);
    signal_.Fire(&m);
  }

  void SendInt(int idx) {
    signal_.Send(idx);
  }

  Slot slot_;
  Signal signal_;
  DummyEventTarget target_;
};

TEST_F(SignalTest, AddListener) {
  AddListener();
}

TEST_F(SignalTest, AddListenerFire) {
  AddListener();
  FireInt(1);
  EXPECT_EQ(target_.GetCalls(1), 1);
}

TEST_F(SignalTest, AddListenerFirePtr) {
  AddListener();
  FireIntPtr(1);
  EXPECT_EQ(target_.GetCalls(1), 1);
}

TEST_F(SignalTest, MultiListenerSend) {
  AddListener();
  Slot s(fastdelegate::MakeDelegate(&target_, &DummyEventTarget::call));
  signal_.AddListener(&s);
  EXPECT_EQ(signal_.GetNumListeners(), 2);
  EXPECT_EQ(s.GetListeningCount(), 1);
  SendInt(1);
  EXPECT_EQ(target_.GetCalls(1), 2);
}

TEST_F(SignalTest, RemoveListener) {
  AddListener();
  signal_.RemoveListener(&slot_);
  EXPECT_EQ(slot_.GetListeningCount(), 0);
  EXPECT_EQ(signal_.GetNumListeners(), 0);
}

TEST_F(SignalTest, RemoveListenerMulti) {
  AddListener();
  Slot s(fastdelegate::MakeDelegate(&target_, &DummyEventTarget::call));
  signal_.AddListener(&s);
  EXPECT_EQ(signal_.GetNumListeners(), 2);
  EXPECT_EQ(s.GetListeningCount(), 1);
  signal_.RemoveListener(&slot_);
  EXPECT_EQ(slot_.GetListeningCount(), 0);
  EXPECT_EQ(s.GetListeningCount(), 1);
  EXPECT_EQ(signal_.GetNumListeners(), 1);
}

}  // namespace gnat
