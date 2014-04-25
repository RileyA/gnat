#include "gnat.h"
#include "game/game.h"
#include "event/message.h"
#include "event/signal.h"

namespace gnat {

Game::Game()
  : input_(NULL),
    gl_(NULL),
    shutdown_(false),
    end_state_(false) {}
//---------------------------------------------------------------------------

void Game::Init(GLPlatformContext* gl, InputPlatformContext* input) {
  gl_ = gl;
  input_ = input;
  CreateSlot("window_closed", this, &Game::WindowClosed);
  CreateSlot("shutdown", this, &Game::Shutdown);
  CreateSlot("end_state", this, &Game::EndState);
}
//---------------------------------------------------------------------------

void Game::Go(GameState *state, size_t w, size_t h, bool fullscreen,
              bool lock_mouse) {
  DCHECK(gl_ && input_);
  gl_->InitDisplay(w, h, fullscreen);
  input_->InitInput(lock_mouse);

  input_->GetSignal("window_closed")
      ->AddListener(GetSlot("window_closed"));

  timer_.Start();
  float last = timer_.GetTimeSeconds();

  if (!state)
    return;

  current_state_ = state;
  current_state_->Init(this);

  while (current_state_) {
    float now = timer_.GetTimeSeconds();
    float delta = now - last;
    last = now;
    current_state_->Update(delta);
    if (shutdown_) {
      current_state_->Deinit();
      delete current_state_;
      return;
    }
    if (end_state_) {
      current_state_->Deinit();
      delete current_state_;
      if (!next_state_)
        return;
      current_state_ = next_state_;
      current_state_->Init(this);
      end_state_ = false;
    }
  }
}
//---------------------------------------------------------------------------

void Game::WindowClosed(const Message& m) {
  shutdown_ = true;
}
//---------------------------------------------------------------------------

void Game::Shutdown(const Message& m) {
  shutdown_ = true;
}
//---------------------------------------------------------------------------

void Game::EndState(const Message& m) {
  next_state_ = message_cast<GameState*>(m);
  end_state_ = true;
}
//---------------------------------------------------------------------------

}  // namespace gnat
