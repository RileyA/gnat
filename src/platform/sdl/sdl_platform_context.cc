#include "sdl_platform_context.h"

#include "event/signal.h"
#include "gfx/math/vector3.h"
#include "platform/sdl/sdl_keycode_conversion.h"

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
//---------------------------------------------------------------------------

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
//---------------------------------------------------------------------------

SDLPlatformContext::~SDLPlatformContext() {}
//---------------------------------------------------------------------------

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
//---------------------------------------------------------------------------

void SDLPlatformContext::UpdateDisplay() {
  HandleEvents();
  SwapBuffers();
}
//---------------------------------------------------------------------------

void SDLPlatformContext::SwapBuffers() {
  SDL_GL_SwapWindow(window_);
}
//---------------------------------------------------------------------------

long SDLPlatformContext::GetWindowHandle() {
  NOTREACHED();
  return 0;
}
//---------------------------------------------------------------------------

void SDLPlatformContext::InitInput(bool lock_mouse) {
  CreateSignal("key_down");
  CreateSignal("key_up");
  CreateSignal("mouse_move");
  CreateSignal("mouse_down");
  CreateSignal("mouse_up");
  if (lock_mouse) {
    SDL_ShowCursor(0);
    SDL_SetRelativeMouseMode((SDL_bool)true);
  }
}
//---------------------------------------------------------------------------

void SDLPlatformContext::UpdateInput() {
  HandleEvents();
}
//---------------------------------------------------------------------------

bool SDLPlatformContext::IsKeyPressed(KeyCode code) {
  return keys_down_.count(code);
}
//---------------------------------------------------------------------------

bool SDLPlatformContext::IsMousePressed(unsigned char index) {
  return mouse_down_.count(index);
}
//---------------------------------------------------------------------------

Signal* SDLPlatformContext::GetKeyUpSignal(KeyCode code) {
  if (key_up_signals_.count(code))
    return key_up_signals_[code];
  key_up_signals_[code] = CreateSignal(String(kKeyNames[code]) + "_UP");
  return key_up_signals_[code];
}
//---------------------------------------------------------------------------

Signal* SDLPlatformContext::GetKeyDownSignal(KeyCode code) {
  if (key_down_signals_.count(code))
    return key_down_signals_[code];
  key_down_signals_[code] = CreateSignal(String(kKeyNames[code]) + "_DOWN");
  return key_down_signals_[code];
}
//---------------------------------------------------------------------------

void SDLPlatformContext::Deinit() {
  if (gl_context_) {
    SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(window_);
  }
  SDL_Quit();
}
//---------------------------------------------------------------------------

void SDLPlatformContext::HandleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        GetSignal("window_closed")->Fire(NULL); break;
      case SDL_KEYDOWN:
        if (input_context_)
          OnKeyDown(event);
        break;
      case SDL_KEYUP:
        if (input_context_)
          OnKeyUp(event);
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (input_context_)
          OnMouseDown(event);
        break;
      case SDL_MOUSEBUTTONUP:
        if (input_context_)
          OnMouseUp(event);
        break;
      case SDL_MOUSEMOTION:
        if (input_context_)
          OnMouseMove(event);
        break;
    }
  }
}
//---------------------------------------------------------------------------

void SDLPlatformContext::OnKeyDown(const SDL_Event& event) {
  DCHECK(event.type == SDL_KEYDOWN);
  KeyCode k = sdl_keysym_to_keycode(event.key.keysym.sym);
  keys_down_.insert(k);
  GetSignal("key_down")->Send(k);
  if (key_down_signals_.count(k))
    key_down_signals_[k]->Send(k);
}
//---------------------------------------------------------------------------

void SDLPlatformContext::OnKeyUp(const SDL_Event& event) {
  DCHECK(event.type == SDL_KEYUP);
  KeyCode k = sdl_keysym_to_keycode(event.key.keysym.sym);
  keys_down_.erase(keys_down_.find(k));
  GetSignal("key_up")->Send(k);
  if (key_up_signals_.count(k))
    key_up_signals_[k]->Send(k);
}
//---------------------------------------------------------------------------

void SDLPlatformContext::OnMouseDown(const SDL_Event& event) {
  DCHECK(event.type == SDL_MOUSEBUTTONDOWN);
  mouse_down_.insert(event.button.button);
  GetSignal("mouse_down")->Send(event.button.button);
}
//---------------------------------------------------------------------------

void SDLPlatformContext::OnMouseUp(const SDL_Event& event) {
  DCHECK(event.type == SDL_MOUSEBUTTONUP);
  mouse_down_.erase(mouse_down_.find(event.button.button));
  GetSignal("mouse_up")->Send(event.button.button);
}
//---------------------------------------------------------------------------

void SDLPlatformContext::OnMouseMove(const SDL_Event& event) {
  Vector3 motion = Vector3(event.motion.xrel, event.motion.yrel, 0.0);
  GetSignal("mouse_move")->Send(motion);
}
//---------------------------------------------------------------------------

}  // namespace gnat
