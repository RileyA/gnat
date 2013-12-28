#ifndef gnat_util_logger_H
#define gnat_util_logger_H

#include "event/event_handler.h"

namespace gnat {
 
/** A simple logger */
class Logger : public EventHandler {
 public:

  Logger();
  virtual ~Logger();

  void log(String message, int level = 0);

 private:
  
};

}  // namespace gnat

#endif

