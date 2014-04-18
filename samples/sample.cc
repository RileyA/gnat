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
  DCHECK(false);
  gnat::GLPlatformContext* ctx = new gnat::SDLPlatformContext(true, false);
  ctx->InitDisplay(800, 600, false);

  ExitListener listen;
  ctx->GetSignal("window_closed")->AddListener(listen.GetSlot("window_closed"));

  gnat::GraphicsContext gfxctx(ctx);
  gfxctx.Init();

  gnat::Camera* c = new gnat::Camera();

  gfxctx.SetMainCamera(c);

  float test = 0.0;
  gnat::ColorF32 color(0.0, 0.0, 0.0, 0.0);

  while(!listen.pressed()) {
    color.r += 0.001f;
    gfxctx.SetClearColor(color);
    gfxctx.Update(0.0);
  }

  ctx->Deinit();
  gfxctx.Deinit();
  delete ctx;

  return 0;
}


