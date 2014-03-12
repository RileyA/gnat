#ifndef gnat_event_signal_H
#define gnat_event_signal_H

#include "gnat.h"
#include "message.h"

namespace gnat {

class Slot;

/** A Qt-inspired Signal, maintains a list of listeners (Slots)
 *    and calls their delegates when fired. */
class Signal {
 public:

  Signal();
  ~Signal();

  /** Fires this signal (calls all listeners' delegates) with a given message
   *    @param message The message to send */
  void Fire(const Message* message) const;

  /** Fires this signal (calls all listeners' delegates) with a given message
   *    @param message The message to send */
  void Fire(const Message& message) const;

  /** Fires this signal (this one handles packing the data magically)
   *    @param data The message to be packed and sent */
  template<class T> void Send(const T& data) const {
    Fire(MessageAny<T>(data));
  }

  /** Adds a listener to this Signal
   *    @param Slot Pointer to the Slot */
  void AddListener(Slot* slot, bool notify=true);

  /** Removes a Listener from this Signal
   *    @param slot Pointer to the desired slot */
  void RemoveListener(Slot* slot, bool notify=true);

  /** Removes all listeners from this Signal */
  void RemoveAllListeners();

  int GetNumListeners() { return listeners_.size(); }

private:
  List<Slot*> listeners_;
};
} // namespace gnat

#endif

