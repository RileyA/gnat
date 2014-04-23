#include "event/observer.h"
#include "event/signal.h"
#include "platform/sdl/sdl_platform_context.h"
#include "gfx/graphics_context.h"
#include "gfx/util/mesh_data.h"
#include "gfx/scene/mesh.h"
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

int main(int argc, char** argv) {
  gnat::FileUtils::SetBaseFilePathFromArgv(argv);

  gnat::GLPlatformContext* ctx = new gnat::SDLPlatformContext(true, false);
  ctx->InitDisplay(800, 600, false);

  ExitListener listen;
  ctx->GetSignal("window_closed")->AddListener(listen.GetSlot("window_closed"));

  gnat::GraphicsContext gfxctx(ctx);
  gfxctx.Init();

  gnat::Camera* c = new gnat::Camera();

  gfxctx.SetMainCamera(c);

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
  d.Append<gnat::Vector3>(gnat::Vector3( 1.0,  1.0, 0.0)); // LR
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
  mat->set_shader(program);

  md->SetMaterial(mat);

  gfxctx.GetRootNode()->AddChild(n);

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


