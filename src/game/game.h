#ifndef gnat_game_game_H
#define gnat_game_game_H

#include "gnat.h"

#include "game/game_state.h"
#include "event/event_handler.h"
#include "platform/input_platform_context.h"
#include "platform/gl_platform_context.h"
#include "gfx/graphics_context.h"
#include "util/timer.h"

namespace gnat {

class Game : public EventHandler {
 public:

  Game();
  virtual ~Game() {}

  void Init(GLPlatformContext* gl, InputPlatformContext* input);
  void Go(GameState *state, size_t w, size_t h, bool fullscreen,
          bool lock_mouse);

  GLPlatformContext* gl() { return gl_; }
  InputPlatformContext* input() { return input_; }

 private:
  
  void Update(Real delta);

  void WindowClosed(const Message& m);
  void Shutdown(const Message& m);
  void EndState(const Message& m);

  GameState* current_state_;
  GameState* next_state_;

  InputPlatformContext* input_;
  GLPlatformContext* gl_;

  Timer timer_;

  bool shutdown_, end_state_;
};

}  // namespace gnat

#endif  // gnat_game_game_H
