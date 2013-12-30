#ifndef gnat_gfx_glx_platform_context_H
#define gnat_gfx_glx_platform_context_H

#include "event/event_handler.h"
#include "event/signal.h"
#include "gfx/gl_platform_context.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

namespace gnat {

class GLXPlatformContext : public GLPlatformContext {
 public:

  GLXPlatformContext();
  ~GLXPlatformContext();

  virtual void Init(size_t h, size_t w, bool fullscreen);
  virtual void Deinit();
  virtual void Update();
  virtual void SwapBuffers();
  virtual long GetWindowHandle();

 private:

  Display* display_;
  Window root_;
  XVisualInfo* visual_;
  Colormap colormap_;
  XSetWindowAttributes set_window_attributes_;
  Window window_;
  GLXContext context_;
  Atom delete_message_;
  bool initialized_;

};

}

#endif
