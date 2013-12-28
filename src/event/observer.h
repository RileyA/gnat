#ifndef gnat_event_observer_H
#define gnat_event_observer_H

#include "slot.h"
#include "third_party/FastDelegate/FastDelegate.h"

namespace gnat {

/** An object that may subscribe callbacks to Observables' signals. */
class Observer {
 public:
  
  Observer();
  virtual ~Observer();

  Slot* GetSlot(String name);
  Slot* CreateSlot(String name, Delegate fn);

  template <typename obj, typename func>
  Slot* CreateSlot(String name, obj context, func fn) {
    return CreateSlot(name, fastdelegate::MakeDelegate(context, fn));
  }

 private:
  
  Map<String, Slot*> slots_;
};

}  // namespace gnat
#endif

