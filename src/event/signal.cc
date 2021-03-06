#include "signal.h"
#include "slot.h"
#include "message.h"

namespace gnat {

Signal::Signal() {}
//---------------------------------------------------------------------------

Signal::~Signal() {
  RemoveAllListeners();
}
//---------------------------------------------------------------------------

void Signal::Fire(const Message* message) const {
  for(List<Slot*>::const_iterator it = listeners_.begin();
      it != listeners_.end(); ++it) {
    (*it)->slot_(*message);
  }
}
//---------------------------------------------------------------------------

void Signal::Fire(const Message& message) const {
  for(List<Slot*>::const_iterator it = listeners_.begin();
      it != listeners_.end(); ++it) {
    (*it)->slot_(message);
  }
}
//---------------------------------------------------------------------------

void Signal::AddListener(Slot* slot, bool notify) {
  listeners_.push_back(slot);
  if (notify) {
    slot->ListenTo(this, false);
  }
}
//---------------------------------------------------------------------------

void Signal::RemoveListener(Slot* slot, bool notify) {
  for(List<Slot*>::iterator it = listeners_.begin();
    it != listeners_.end(); ++it) {
    if((*it) == slot) {
      if(notify) {
        (*it)->StopListeningTo(this, false);
      }
      listeners_.erase(it);
      return;
    }
  }
}
//---------------------------------------------------------------------------

void Signal::RemoveAllListeners() {
  for(List<Slot*>::iterator it = listeners_.begin();
    it != listeners_.end(); ++it) {
    (*it)->StopListeningTo(this, false);
  }
  listeners_.clear();
}
//---------------------------------------------------------------------------

} // namespace gnat

