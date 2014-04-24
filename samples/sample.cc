#include "event/observer.h"
#include "event/signal.h"
#include "platform/sdl/sdl_platform_context.h"
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

class FPSCamera : public gnat::EventHandler {
 public:
  FPSCamera() {
    CreateSlot("mouse_moved", this, &FPSCamera::MouseMoved);
    CreateSlot("key_down", this, &FPSCamera::KeyDown);
    CreateSlot("key_up", this, &FPSCamera::KeyUp);
    position_.AddChild(&yaw_);
    yaw_.AddChild(&pitch_);
    pitch_.AddChild(&roll_);
    roll_.AddChild(&camera_);
    w = a = s = d = false;
  }

  void MouseMoved(const gnat::Message& m) {
    gnat::Vector3 move = gnat::message_cast<gnat::Vector3>(m);

    y -= 0.002 * move.x;
    p -= 0.002 * move.y;

    gnat::Quaternion q = gnat::Quaternion::IDENTITY;
    q.FromAngleAxis(y, gnat::Vector3(0, 1, 0));
    yaw_.SetOrientation(q);
    q.FromAngleAxis(p, gnat::Vector3(1, 0, 0));
    pitch_.SetOrientation(q);
  }

  void Update() {
    float forward = - 1 * s + 1 * w;
    float right = - 1 * a + 1 * d;
    if (w || s || a || d)
      position_.SetPosition(position_.GetPosition() +
                          GetDirection() * forward * 0.01f +
                          GetRight() * right * 0.01f);
    gnat::Vector3 d = GetDirection();
    printf("Dir: %f %f %f\n", d.x, d.y, d.z);
  }

  void KeyDown(const gnat::Message& m) {
    SDL_Scancode sc = gnat::message_cast<SDL_Scancode>(m);
    if (sc == SDL_SCANCODE_W)
      w = true;
    if (sc == SDL_SCANCODE_S)
      s = true;
    if (sc == SDL_SCANCODE_A)
      a = true;
    if (sc == SDL_SCANCODE_D)
      d = true;
  }

  void KeyUp(const gnat::Message& m) {
    SDL_Scancode sc = gnat::message_cast<SDL_Scancode>(m);
    if (sc == SDL_SCANCODE_W)
      w = false;
    if (sc == SDL_SCANCODE_S)
      s = false;
    if (sc == SDL_SCANCODE_A)
      a = false;
    if (sc == SDL_SCANCODE_D)
      d = false;
  }

  gnat::Vector3 GetDirection() {
    return camera_.GetTransform().ExtractQuaternion() * gnat::Vector3::NEGATIVE_UNIT_Z;
  }

  gnat::Vector3 GetRight() {
    return camera_.GetTransform().ExtractQuaternion() * gnat::Vector3::UNIT_X;
  }

  bool w, a, s, d;
  float y;
  float p;
  gnat::Camera camera_;
  gnat::Node position_;
  gnat::Node yaw_;
  gnat::Node pitch_;
  gnat::Node roll_;
};

int main(int argc, char** argv) {
  gnat::FileUtils::SetBaseFilePathFromArgv(argv);

  gnat::SDLPlatformContext *sdl = new gnat::SDLPlatformContext(true, true);
  gnat::GLPlatformContext* ctx = sdl;
  ctx->InitDisplay(800, 600, false);
  sdl->InitInput();

  //FPSCamera* camera = new FPSCamera();
  gnat::FPSCamera* camera = new gnat::FPSCamera();

  ExitListener listen;
  ctx->GetSignal("window_closed")->AddListener(listen.GetSlot("window_closed"));
  sdl->GetSignal("mouse_move")->AddListener(camera->GetSlot("mouse_moved"));
  sdl->GetSignal("key_down")->AddListener(camera->GetSlot("key_down"));
  sdl->GetSignal("key_up")->AddListener(camera->GetSlot("key_up"));

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

  while(!listen.pressed()) {
    sig->Send(double(0.01));
    if (sdl->IsKeyPressed(SDL_SCANCODE_W)) {
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


