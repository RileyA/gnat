#ifndef gnat_event_observer_H
#define gnat_event_observer_H

#include "slot.h"

namespace gnat {

/** An object that may be subscribe callbacks to Observables' signals. */
class Observer {
 public:
  
  Observer();
  virtual ~Observer();

  Slot* CreateSlot(String name, Delegate fn);

  template <typename obj, typename func>
  Slot* createSlot(String name, obj context, func fn)
  {
    return CreateSlot(name, fastdelegate::MakeDelegate(context, fn));
  }

 private:
  
  Map<String, Slot> slots_;
};
}

#endif

