#ifndef gnat_util_logger_H
#define gnat_util_logger_H

#include "event/event_handler.h"

namespace gnat {
 
/** A simple logger */
class Logger : public EventHandler {
 public:

  Logger();
  virtual ~Logger();

  void log();
  void log(int level);

 private:
  
}

}

#endif

