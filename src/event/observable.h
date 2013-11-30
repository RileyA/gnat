#ifndef gnat_event_observable_H
#define gnat_event_observable_H

namespace gnat {

/** An object that can emit signals that may be observed by Observers. */
class Observable {
 public:
  
  Observable();
  virtual ~Observable();

  void CreateSignal(String name);
  Signal* GetSignal(String name);

 private:
  
  Map<String, Signal*> signals_;

};

} // namespace gnat

#endif

