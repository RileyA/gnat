#ifndef gnat_gfx_gl_platform_context_H
#define gnat_gfx_gl_platform_context_H

#include "gnat.h"
#include "event_handler.h"

namespace gnat {

/** Interface to be implemented by platform-specific GL context 
 *		stuffs (i.e. Xlib/GLX in Linux). */
class GLPlatformContext : public EventHandler {
 public:

  virtual void init(size_t h, size_t w, bool fullscreen) = 0;
  virtual void deinit() = 0;
  virtual void update() = 0;
  virtual void swapBuffers() = 0;
  virtual long getWindowHandle() = 0;

  size_t height() { return height_; }
  size_t width() { return width_; }

 protected:

  size_t width_, height_;

};

}

#endif
