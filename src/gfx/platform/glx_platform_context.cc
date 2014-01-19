#include "glx_platform_context.h"

namespace gnat {

GLXPlatformContext::GLXPlatformContext() {
  initialized_ = false;
  display_ = 0;
  visual_ = 0;
  CreateSignal("WindowResized");
  CreateSignal("WindowClosed");
}
//-----------------------------------------------------------------------

GLXPlatformContext::~GLXPlatformContext() {
  Deinit();
}
//-----------------------------------------------------------------------

void GLXPlatformContext::Init(size_t w, size_t h, bool fullscreen) {
  if(!initialized_) {
    // open local display
    display_ = XOpenDisplay(0);

    if(!display_)
      return;

    root_ = DefaultRootWindow(display_);

    GLint attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    visual_ = glXChooseVisual(display_, 0, attributes);

    if(!visual_)
      return;

    colormap_ = XCreateColormap(display_, root_, visual_->visual, AllocNone);

    set_window_attributes_.event_mask = StructureNotifyMask;
    set_window_attributes_.colormap = colormap_;

    window_ = XCreateWindow(display_, root_, 0, 0, w, h, 0, visual_->depth,
      InputOutput, visual_->visual, CWColormap | CWEventMask, &set_window_attributes_);

    // register interest in the delete window message
    delete_message_ = XInternAtom(display_, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display_, window_, &delete_message_, 1);

    XMapWindow(display_, window_);
    XStoreName(display_, window_, "Test");

    context_ = glXCreateContext(display_, visual_, 0, GL_TRUE);
    glXMakeCurrent(display_, window_, context_);

    // the window and context are now all initialized, yay!
    initialized_ = true;
  }
}
//-----------------------------------------------------------------------

void GLXPlatformContext::Deinit() {
  if(initialized_) {
    glXMakeCurrent(display_, None, 0);
    glXDestroyContext(display_, context_);
    XDestroyWindow(display_, window_);
    XCloseDisplay(display_);
    initialized_ = false;
  }
}
//-----------------------------------------------------------------------

void GLXPlatformContext::Update() {
  XEvent xev;

  while(XCheckMaskEvent(display_, StructureNotifyMask, &xev)) {
    switch(xev.type) {
      case ConfigureNotify:

        if(xev.xconfigure.width != width_ || xev.xconfigure.height != height_) {
          width_ = xev.xconfigure.width;
          height_ = xev.xconfigure.height;
          std::pair<size_t, size_t> report = std::make_pair(width_, height_);
          GetSignal("WindowResized")->Send(report);
        }

        break;

      case ClientMessage:

        if(xev.xclient.data.l[0] == delete_message_) {
          GetSignal("WindowClosed")->Send(xev.xclient.data.l[0]);
        }

        break;
    }
  }

  while (XCheckTypedEvent(display_, ClientMessage, &xev)) {
    if(xev.xclient.data.l[0] == delete_message_) {
      if (GetSignal("WindowClosed")->GetNumListeners()) {
        GetSignal("WindowClosed")->Send(xev.xclient.data.l[0]);
      } else {
        Deinit();
        return;
      }
    }
  }
}
//-----------------------------------------------------------------------

void GLXPlatformContext::SwapBuffers() {
  glXSwapBuffers(display_, window_); 
}
//-----------------------------------------------------------------------

long GLXPlatformContext::GetWindowHandle() {
  return window_;
}
//-----------------------------------------------------------------------

}
