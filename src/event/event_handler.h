#ifndef gnat_event_event_handler_H
#define gnat_event_event_handler_H

#include "gnat.h"
#include "observer.h"
#include "observable.h"

namespace gnat {

/** This is simply a combination of Observer and Observable,
 *    it is used for anything that should be playing both of
 *    the observer pattern roles. */
class EventHandler : public Observer, public Observable {};

}  // namespace gnat

#endif
