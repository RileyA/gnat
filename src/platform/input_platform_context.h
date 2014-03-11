#ifndef gnat_platform_input_platform_context_H
#define gnat_platform_input_platform_context_H

#include "event/event_handler.h"

namespace gnat {

/** Interface to platform-specific input handling stuff. */
class InputPlatformContext : virtual public EventHandler {
 public:
  virtual void InitInput() = 0;
  virtual void UpdateInput() = 0;
  virtual bool IsKeyPressed() = 0;
  virtual void Deinit() = 0;
};

}  // namespace gnat

#endif
