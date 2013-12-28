#include "slot.h"
#include "signal.h"

namespace gnat
{

void Slot::ListenTo(Signal* signal, bool notify) {
  signals_.push_back(signal);
  if(notify)
    signal->AddListener(this,false);
}
//-----------------------------------------------------------------------

void Slot::StopListeningTo(Signal* signal, bool notify) {
  List<Signal*>::iterator it = signals_.begin();
  for(it; it != signals_.end(); ++it) {
    if(signal == (*it)) {
      if(notify)
        (*it)->RemoveListener(this,false);
      signals_.erase(it);
      return;
    }
  }
}
//-----------------------------------------------------------------------

void Slot::StopAllListening() {
  List<Signal*>::iterator it = signals_.begin();
  for(it; it != signals_.end(); ++it) {
    (*it)->RemoveListener(this);
  }
  signals_.clear();
}
//-----------------------------------------------------------------------

}  // namespace gnat
