#ifndef gnat_game_game_state_H
#define gnat_game_game_state_H

#include "gnat.h"

#include "event/event_handler.h"

namespace gnat {

class Game;

class GameState : public EventHandler {
 public:
  virtual void Init(Game* game) = 0;
  virtual void Deinit() = 0;
  virtual void Update(double delta) = 0;
};

}  // namespace gnat

#endif  // gnat_game_game_state_H
