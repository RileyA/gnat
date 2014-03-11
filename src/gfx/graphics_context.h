#ifndef gnat_gfx_graphics_context_H
#define gnat_gfx_graphics_context_H

#include "gnat.h"
#include "event/event_handler.h"

namespace gnat {

class GLPlatformContext;

class GraphicsContext : public EventHandler {
 public:

  GraphicsContext(GLPlatformContext* platform_context);
  ~GraphicsContext();

  void Update(Real delta);

 private:

  GLPlatformContext* platform_context_;
};

}  // namespace gnat

#endif  // gnat_gfx_graphics_context_H
