#ifndef gnat_event_dummy_event_target_H
#define gnat_event_dummy_event_target_H

#include "gnat.h"

#include "event/slot.h"
#include "event/message.h"

namespace gnat {

class DummyEventTarget {
 public:
  void call(const Message& m) {
    int idx = gnat::message_cast<int>(m);
    if (calls_.count(idx))
      ++calls_[idx];
    else
      calls_[idx] = 1;
  }

  int GetCalls(int idx) {
    return calls_[idx];
  }

  int GetTotalCalls() {
    int calls = 0;
    for (Map<int, int>::iterator it = calls_.begin(); it != calls_.end();
         ++it) {
      calls += it->second;
    }
    return calls;
  }

 private:

  // idx, count
  Map<int, int> calls_;
};

}  // namespace gnat

#endif  // gnat_event_dummy_event_target_H
