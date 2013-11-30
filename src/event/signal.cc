#include "signal.h"
#include "message.h"

namespace gnat {

Signal::~Signal() {
  RemoveAllListeners();
}

void Signal::Fire(const Message* message) const {
  for(list<Slot*>::const_iterator it = listeners_.begin();
      it != listeners_.end(); ++it) {
    (*it)->slot(*message);
  }
}

void Signal::Fire(const Message& message) const {
  for(list<Slot*>::const_iterator it = listeners_.begin();
      it != listeners_.end(); ++it) {
    (*it)->slot(message);
  }
}

void Signal::AddListener(Slot* slot, bool notify) {
  listeners_.push_back(slot);
  if (notify) {
    slot->ListenTo(this);
  }
}

void Signal::RemoveListener(Slot* slot, bool notify) {
  for(list<Slot*>::iterator it = listeners_.begin();
    it != listeners_.end(); ++it) {
    if((*it)==slot) {
      if(notify) {
        (*it)->StopListeningTo(this, false);
      }
      listeners_.erase(it);
      return;
    }
  }
}

void Signal::RemoveAllListeners() {
  for(list<Slot*>::iterator it = listeners_.begin();
    it != listeners_.end(); ++it) {
    (*it)->StopListeningTo(this,false);
  }
  listeners_.clear();
}

} // namespace gnat

