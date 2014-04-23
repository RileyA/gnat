#include "sdl_platform_context.h"

#include "event/signal.h"

#include <iostream>

namespace gnat {

#define SDL_ERROR() check_error(__LINE__, __FILE__)
#define SDL_CHECK_ERROR() DCHECK(!check_error(__LINE__, __FILE__))
bool check_error(int line, const char* filename) {
#ifndef NDEBUG
	const char* error = SDL_GetError();
	if (*error != '\0') {
		printf("SDL Error: %s %s:%d\n", error, filename, line); SDL_ClearError();
    return true;
	}
  return false;
#endif
}

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

  width_ = w;
  height_ = h;

  // TODO: GL version handling stuffs
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  window_ = SDL_CreateWindow(APPLICATION_NAME, SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,w, h,
                                (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) 
                                | SDL_WINDOW_OPENGL);
  SDL_ERROR();
  if (!window_) {
    // TODO: settle on a consistent error handling scheme for cases like this
    exit(1);
  }

  context_ = SDL_GL_CreateContext(window_);
  SDL_CHECK_ERROR();

  GLenum err = glewInit();

  if (err != GLEW_OK) {
    printf("Glew Init Error: ", glewGetErrorString(err));
    exit(1);
  }

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
  CreateSignal("key_down");
  CreateSignal("key_up");
}

void SDLPlatformContext::UpdateInput() {
  HandleEvents();
}

bool SDLPlatformContext::IsKeyPressed(uint32_t code) {
  return keys_down_.count((SDL_Scancode)code);
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
      case SDL_KEYDOWN:
        if (input_context_)
          OnKeyDown(event);
        break;
      case SDL_KEYUP:
        if (input_context_)
          OnKeyUp(event);
        break;
    }
  }
}

void SDLPlatformContext::OnKeyDown(const SDL_Event& event) {
  DCHECK(event.type == SDL_KEYDOWN);
  keys_down_.insert(event.key.keysym.scancode);
  GetSignal("key_down")->Send(event.key.keysym.scancode);
}

void SDLPlatformContext::OnKeyUp(const SDL_Event& event) {
  DCHECK(event.type == SDL_KEYUP);
  keys_down_.erase(keys_down_.find(event.key.keysym.scancode));
  GetSignal("key_up")->Send(event.key.keysym.scancode);
}

}  // namespace gnat
