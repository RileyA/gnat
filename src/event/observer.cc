#include "observer.h"

namespace gnat {

Observer::Observer() {
  
}
//---------------------------------------------------------------------------

Observer::~Observer() {
  for (Map<String, Slot*>::iterator it = slots_.begin(); it != slots_.end();
       ++it) {
    delete it->second;
  }
  slots_.clear();
}
//---------------------------------------------------------------------------

Slot* Observer::GetSlot(String name) {
  Map<String, Slot*>::iterator it = slots_.find(name);
  return it != slots_.end() ? it->second : 0;
}
//---------------------------------------------------------------------------

Slot* Observer::CreateSlot(String name, Delegate fn) {
  if (!GetSlot(name)) {
    slots_[name] = new Slot(fn);
    return slots_[name];
  }
  return 0;
}
//---------------------------------------------------------------------------

}  // namespace gnat
