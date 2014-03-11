#ifndef gnat_platform_platform_context_H
#define gnat_platform_platform_context_H

#include "gnat.h"
#include "event/event_handler.h"

namespace gnat {

/** Interface for common platform-specific functionality (generally window
 * system-y stuff, GL ctx creation, input handling, perhaps audio, etc). */
class PlatformContext : public EventHandler {
 public:
  virtual void Init() = 0;
  virtual void Deinit() = 0;
};

}

#endif
