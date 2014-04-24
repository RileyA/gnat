#include "event/observer.h"
#include "event/signal.h"
#include "platform/sdl/sdl_platform_context.h"
#include "platform/keycodes.h"
#include "gfx/graphics_context.h"
#include "gfx/util/mesh_data.h"
#include "gfx/math/quaternion.h"
#include "gfx/scene/mesh.h"
#include "gfx/scene/fps_camera.h"
#include "gfx/scene/mesh_drawable.h"
#include "gfx/scene/node.h"
#include "gfx/material/material.h"
#include "gfx/material/program.h"
#include "gfx/material/shader.h"
#include "gfx/scene/scene_manager.h"
#include "util/file_utils.h"
#include "util/timer.h"

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
  gnat::FileUtils::SetBaseFilePathFromArgv(argv);

  gnat::SDLPlatformContext *sdl = new gnat::SDLPlatformContext(true, true);
  gnat::GLPlatformContext* ctx = sdl;
  ctx->InitDisplay(800, 600, false);
  sdl->InitInput(true);

  //FPSCamera* camera = new FPSCamera();
  gnat::FPSCamera* camera = new gnat::FPSCamera();

  ExitListener listen;
  ctx->GetSignal("window_closed")->AddListener(listen.GetSlot("window_closed"));
  sdl->GetSignal("mouse_move")->AddListener(camera->GetSlot("mouse_moved"));

  sdl->GetKeyUpSignal(gnat::K_W)->AddListener(camera->GetSlot("key_up"));
  sdl->GetKeyUpSignal(gnat::K_A)->AddListener(camera->GetSlot("key_up"));
  sdl->GetKeyUpSignal(gnat::K_S)->AddListener(camera->GetSlot("key_up"));
  sdl->GetKeyUpSignal(gnat::K_D)->AddListener(camera->GetSlot("key_up"));
  sdl->GetKeyDownSignal(gnat::K_W)->AddListener(camera->GetSlot("key_down"));
  sdl->GetKeyDownSignal(gnat::K_A)->AddListener(camera->GetSlot("key_down"));
  sdl->GetKeyDownSignal(gnat::K_S)->AddListener(camera->GetSlot("key_down"));
  sdl->GetKeyDownSignal(gnat::K_D)->AddListener(camera->GetSlot("key_down"));

  gnat::Signal* sig = new gnat::Signal();
  camera->GetSlot("tick")->ListenTo(sig);

  gnat::GraphicsContext gfxctx(ctx);
  gfxctx.Init();

  gfxctx.SetMainCamera(camera->GetCamera());

  gnat::MeshData d;
  d.AddAttribute("position", 3, GL_FLOAT);
  d.AddAttribute("normal", 3, GL_FLOAT);
  d.Start(4, 6);
  d.Append<gnat::Vector3>(gnat::Vector3(-1.0,  1.0, 0.0)); // UL
  d.Append<gnat::Vector3>(gnat::Vector3( 0.0,  1.0, 0.0));
  d.FinishVertex();
  d.Append<gnat::Vector3>(gnat::Vector3(-1.0, -1.0, 0.0)); // LL
  d.Append<gnat::Vector3>(gnat::Vector3( 1.0,  0.0, 0.0));
  d.FinishVertex();
  d.Append<gnat::Vector3>(gnat::Vector3( 1.0, -1.0, 0.0)); // LR
  d.Append<gnat::Vector3>(gnat::Vector3( 0.0,  0.0, 1.0));
  d.FinishVertex();
  d.Append<gnat::Vector3>(gnat::Vector3( 1.0,  1.0, 0.0)); // UR
  d.Append<gnat::Vector3>(gnat::Vector3( 0.0,  0.5, 0.5));
  d.FinishVertex();
  d.AddTriangle(0, 1, 2);
  d.AddTriangle(0, 2, 3);
  gnat::Mesh* m = new gnat::Mesh(&d);
  gnat::MeshDrawable* md = new gnat::MeshDrawable(m);
  gnat::Node* n = new gnat::Node();
  n->AddDrawable(md);

  gnat::Material* mat = new gnat::Material("test");

  gnat::VertexShader *vs = new gnat::VertexShader(
      "testvs", gnat::FileUtils::ReadFile(gnat::FileUtils::GetBaseFilePath() +
                                          "../data/materials/vs.glsl"));
  gnat::FragmentShader *fs = new gnat::FragmentShader(
      "testfs", gnat::FileUtils::ReadFile(gnat::FileUtils::GetBaseFilePath() +
                                          "../data/materials/fs.glsl"));

  vs->Compile();
  fs->Compile();

  gnat::Program* program = new gnat::Program(vs, fs);
  program->Link();
  program->RegisterVertexAttribute("position");
  program->RegisterVertexAttribute("normal");
  program->RegisterAutoUniform("p", gnat::Program::PROJECTION_MATRIX);
  program->RegisterAutoUniform("v", gnat::Program::VIEW_MATRIX);
  program->RegisterAutoUniform("m", gnat::Program::WORLD_MATRIX);
  program->RegisterAutoUniform("mvp", gnat::Program::MVP_MATRIX);
  mat->set_shader(program);

  md->SetMaterial(mat);

  gfxctx.GetRootNode()->AddChild(n);

  float test = 0.0;
  gnat::ColorF32 color(0.0, 0.0, 0.0, 0.0);

  n->SetPosition(gnat::Vector3(0, 0.5, 0));
  camera->SetPosition(gnat::Vector3(0,0,2));

  gnat::Timer t;
  t.Start();

  float last = t.GetTimeSeconds();

  while(!listen.pressed()) {
    float now = t.GetTimeSeconds();
    float delta = now - last;
    last = now;
    sig->Send(double(delta));
    if (sdl->IsKeyPressed(gnat::K_W)) {
      color.r += 0.01f;
    }
    gfxctx.SetClearColor(color);
    gfxctx.Update(0.0);
  }

  ctx->Deinit();
  gfxctx.Deinit();
  delete ctx;

  return 0;
}


