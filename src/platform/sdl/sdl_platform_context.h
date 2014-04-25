#ifndef gnat_platform_sdl_sdl_platform_context
#define gnat_platform_sdl_sdl_platform_context

#include "gnat.h"
#include "platform/input_platform_context.h"
#include "platform/gl_platform_context.h"
#include "glew.h"
#include "SDL.h"
#include "SDL_opengl.h"

namespace gnat {

enum MouseButton {
};

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
  virtual void InitInput(bool lock_mouse);
  virtual void UpdateInput();
  virtual bool IsKeyPressed(KeyCode code);
  virtual bool IsMousePressed(unsigned char index);
  virtual Signal* GetKeyUpSignal(KeyCode code);
  virtual Signal* GetKeyDownSignal(KeyCode code);

  // Shared fns
  virtual void Deinit();

 private:

  void HandleEvents();

  void OnKeyDown(const SDL_Event& event);
  void OnKeyUp(const SDL_Event& event);

  void OnMouseDown(const SDL_Event& event);
  void OnMouseUp(const SDL_Event& event);

  void OnMouseMove(const SDL_Event& event);

  KeyCode ConvertSDLKeycode(SDL_Keycode k);

  SDL_Window* window_;
  SDL_GLContext context_;

  bool gl_context_;
  bool input_context_;

  Set<KeyCode> keys_down_;
  Set<unsigned char> mouse_down_;
  Map<KeyCode, Signal*> key_down_signals_;
  Map<KeyCode, Signal*> key_up_signals_;
};

}  // namespace gnat

#endif  // gnat_platform_sdl_sdl_platform_context
