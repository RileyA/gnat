#include "observable.h"
#include "signal.h"

namespace gnat {

Observable::Observable() {}
//---------------------------------------------------------------------------

Observable::~Observable() {
  for (Map<String, Signal*>::iterator it = signals_.begin();
       it != signals_.end(); ++it) {
    delete it->second;
  }
  signals_.clear();
}
//---------------------------------------------------------------------------

Signal* Observable::CreateSignal(String name) {
  if (!GetSignal(name)) {
    signals_[name] = new Signal();
  }
  return signals_[name];
}
//---------------------------------------------------------------------------

Signal* Observable::GetSignal(String name) {
  Map<String, Signal*>::iterator it = signals_.find(name);
  return it != signals_.end() ? it->second : 0; 
}
//---------------------------------------------------------------------------

} // namespace gnat

