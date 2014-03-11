#include "sdl_platform_context.h"
#include "event/signal.h"

namespace gnat {

  SDLPlatformContext::SDLPlatformContext(bool gl, bool input)
    : gl_context_(gl),
      input_context_(input) {
    uint32_t flags = 0;
    if (gl_context_)
      flags |= SDL_INIT_VIDEO;
    if (input_context_)
      flags |= SDL_INIT_JOYSTICK;

    // If we're not being used at all, why did we get instantiated in the first
    // place?
    DCHECK(flags);
    
    if (SDL_Init(flags) < 0)
      exit(1);
  }

  SDLPlatformContext::~SDLPlatformContext() {}

  void SDLPlatformContext::InitDisplay(size_t w, size_t h, bool fullscreen) {
    DCHECK(gl_context_);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    window_ = SDL_CreateWindow(APPLICATION_NAME, SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,w, h,
                                  (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) 
                                  | SDL_WINDOW_OPENGL);
    if (!window_) {
      // TODO: settle on a consistent error handling scheme for cases like this
      exit(1);
    }

    context_ = SDL_GL_CreateContext(window_);

    CreateSignal("window_closed");
    CreateSignal("window_resized");
  }

  void SDLPlatformContext::UpdateDisplay() {
    HandleEvents();
    SwapBuffers();
  }

  void SDLPlatformContext::SwapBuffers() {
    SDL_GL_SwapWindow(window_);
  }

  long SDLPlatformContext::GetWindowHandle() {
    NOTREACHED();
    return 0;
  }

  void SDLPlatformContext::InitInput() {
  }

  void SDLPlatformContext::UpdateInput() {
    HandleEvents();
  }

  bool SDLPlatformContext::IsKeyPressed() {
    NOTREACHED();
    return false;
  }

  void SDLPlatformContext::Deinit() {
    if (gl_context_) {
      SDL_GL_DeleteContext(context_);
      SDL_DestroyWindow(window_);
    }
    SDL_Quit();
  }

  void SDLPlatformContext::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          GetSignal("window_closed")->Fire(NULL);
          break;
      }
    };
  }
}
