#ifndef gnat_platform_sdl_sdl_platform_context
#define gnat_platform_sdl_sdl_platform_context

#include "gnat.h"
#include "platform/input_platform_context.h"
#include "platform/gl_platform_context.h"
#include "glew.h"
#include "SDL.h"
#include "SDL_opengl.h"

namespace gnat {

class SDLPlatformContext : public InputPlatformContext,
                           public GLPlatformContext {
 public:

  SDLPlatformContext(bool gl, bool input);
  virtual ~SDLPlatformContext();

  // GLPlatformContext impl
  virtual void InitDisplay(size_t w, size_t h, bool fullscreen);
  virtual void UpdateDisplay();
  virtual void SwapBuffers();
  virtual long GetWindowHandle();

  // InputPlatformContext impl
  virtual void InitInput();
  virtual void UpdateInput();
  virtual bool IsKeyPressed(uint32_t code);

  // Shared fns
  virtual void Deinit();

 private:

  void HandleEvents();

  void OnKeyDown(const SDL_Event& event);
  void OnKeyUp(const SDL_Event& event);

  SDL_Window* window_;
  SDL_GLContext context_;

  bool gl_context_;
  bool input_context_;

  Set<SDL_Scancode> keys_down_;
};

}  // namespace gnat

#endif  // gnat_platform_sdl_sdl_platform_context
