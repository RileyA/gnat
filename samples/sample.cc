#include <iostream>
#include "event/observer.h"
#include "gfx/platform/glx_platform_context.h"

class ExitListener : public gnat::Observer {
 public:

  ExitListener() {
    CreateSlot("WindowClosed", this, &ExitListener::ExitPressed);
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
  std::cout << "Test!\n";
  gnat::GLPlatformContext* ctx = new gnat::GLXPlatformContext();
  ctx->Init(800, 600, false);
  ExitListener listen;
  ctx->GetSignal("WindowClosed")->AddListener(listen.GetSlot("WindowClosed"));

  while(!listen.pressed()) {
    ctx->Update();
    ctx->SwapBuffers();
  }

  ctx->Deinit();

  return 0;
}
