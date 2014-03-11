#include "event/observer.h"
#include "event/signal.h"
#include "platform/sdl/sdl_platform_context.h"
#include "gfx/graphics_context.h"

class ExitListener : public gnat::Observer {
 public:

  ExitListener() {
    CreateSlot("window_closed", this, &ExitListener::ExitPressed);
    pressed_ = false;
  }

  void ExitPressed(const gnat::Message& m) {
    pressed_ = true;
  }

  bool pressed() { return pressed_; }

 private:

  bool pressed_;
};

int main(int argc, char** argv) {
  gnat::GLPlatformContext* ctx = new gnat::SDLPlatformContext(true, false);
  ctx->InitDisplay(800, 600, false);

  ExitListener listen;
  ctx->GetSignal("window_closed")->AddListener(listen.GetSlot("window_closed"));

  gnat::GraphicsContext gfxctx(ctx);

  while(!listen.pressed()) {
    gfxctx.Update(0.0);
  }

  ctx->Deinit();
  delete ctx;

  return 0;
}


