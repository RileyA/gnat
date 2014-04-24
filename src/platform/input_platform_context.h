#ifndef gnat_platform_input_platform_context_H
#define gnat_platform_input_platform_context_H

#include "event/event_handler.h"
#include "platform/keycodes.h"

namespace gnat {

/** Interface to platform-specific input handling stuff. */
class InputPlatformContext : virtual public EventHandler {
 public:
  virtual void InitInput(bool lock_mouse) = 0;
  virtual void UpdateInput() = 0;
  virtual bool IsKeyPressed(KeyCode code) = 0;
  virtual bool IsMousePressed(unsigned char index) = 0;
  virtual Signal* GetKeyUpSignal(KeyCode code) = 0;
  virtual Signal* GetKeyDownSignal(KeyCode code) = 0;
  virtual void Deinit() = 0;
};

}  // namespace gnat

#endif
