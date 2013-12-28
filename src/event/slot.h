#ifndef gnat_event_slot_H
#define gnat_event_slot_H

#include "gnat.h"
#include "third_party/FastDelegate/FastDelegate.h"

namespace gnat {

class Message;
class Signal;

typedef fastdelegate::FastDelegate1<const Message&> Delegate;

/** A Qt-inspired Slot, recieves messages sent from Signals */
class Slot
{
public:

  Slot() {}
  Slot(Delegate slot) : slot_(slot) {}

  ~Slot() { StopAllListening(); }

  /** Listen to a given Signal 
   *		@param signal The signal to listen to */
  void ListenTo(Signal* signal, bool notify = true);

  /** Stop listening to a given Signal 
   *		@param signal The signal to stop listening to */
  void StopListeningTo(Signal* signal, bool notify = true);

  /** Stop listening to all Signals */
  void StopAllListening();

private:
  
  // List of Signals being listened to.
  List<Signal*> signals_;

  // Function to call when signalled.
  Delegate slot_;

  // So signal can call |slot_|
  friend class Signal;
};

}  // namespace gnat

#endif
