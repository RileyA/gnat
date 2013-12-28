#ifndef gnat_event_observable_H
#define gnat_event_observable_H

#include "gnat.h"

namespace gnat {

class Signal;

/** An object that can emit signals that may be observed by Observers. */
class Observable {
 public:
  
  Observable();
  virtual ~Observable();

  /** Create a signal by name, no-op if such a signal already exists.
   *    @param name The name. */
  Signal* CreateSignal(String name);

  /** Get a signal by name, returns NULL if a signal by the supplied name
   *   doesn't exist. */
  Signal* GetSignal(String name);

 private:
  Map<String, Signal*> signals_;
};

} // namespace gnat

#endif

