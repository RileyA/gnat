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
#include "game/game.h"
#include "game/game_state.h"

#include "util/scoped_ptr.h"

using namespace gnat;

class TestState : public GameState {
 public:
  TestState() {}

  virtual void Init(Game* game) {
    game_ = game;
    gfx_ = scoped_ptr<GraphicsContext>(new GraphicsContext(game_->gl()));
    gfx_->Init();
    camera_ = new FPSCamera();
    gfx_->SetMainCamera(camera_->GetCamera());
    game_->input()->GetKeyUpSignal(gnat::K_W)
        ->AddListener(camera_->GetSlot("key_up"));
    game_->input()->GetKeyUpSignal(gnat::K_A)
        ->AddListener(camera_->GetSlot("key_up"));
    game_->input()->GetKeyUpSignal(gnat::K_S)
        ->AddListener(camera_->GetSlot("key_up"));
    game_->input()->GetKeyUpSignal(gnat::K_D)
        ->AddListener(camera_->GetSlot("key_up"));
    game_->input()->GetKeyDownSignal(gnat::K_W)
        ->AddListener(camera_->GetSlot("key_down"));
    game_->input()->GetKeyDownSignal(gnat::K_A)
        ->AddListener(camera_->GetSlot("key_down"));
    game_->input()->GetKeyDownSignal(gnat::K_S)
        ->AddListener(camera_->GetSlot("key_down"));
    game_->input()->GetKeyDownSignal(gnat::K_D)
        ->AddListener(camera_->GetSlot("key_down"));
    game_->input()->GetSignal("mouse_move")
        ->AddListener(camera_->GetSlot("mouse_moved"));
    gfx_->SetClearColor(ColorF32(1.0, 0.3, 0.2, 1.0));
    Signal* tick = CreateSignal("tick");
    camera_->GetSlot("tick")->ListenTo(tick);

    MeshData d;
    d.AddAttribute("position", 3, GL_FLOAT);
    d.AddAttribute("normal", 3, GL_FLOAT);
    d.Start(4, 6);
    d.Append<Vector3>(Vector3(-1.0,  1.0, 0.0)); // UL
    d.Append<Vector3>(Vector3( 0.0,  1.0, 0.0));
    d.FinishVertex();
    d.Append<Vector3>(Vector3(-1.0, -1.0, 0.0)); // LL
    d.Append<Vector3>(Vector3( 1.0,  0.0, 0.0));
    d.FinishVertex();
    d.Append<Vector3>(Vector3( 1.0, -1.0, 0.0)); // LR
    d.Append<Vector3>(Vector3( 0.0,  0.0, 1.0));
    d.FinishVertex();
    d.Append<Vector3>(Vector3( 1.0,  1.0, 0.0)); // UR
    d.Append<Vector3>(Vector3( 0.0,  0.5, 0.5));
    d.FinishVertex();
    d.AddTriangle(0, 1, 2);
    d.AddTriangle(0, 2, 3);
    Mesh* m = new Mesh(&d);
    MeshDrawable* md = new MeshDrawable(m);
    Node* n = new Node();
    n->AddDrawable(md);

    Material* mat = new Material("test");
    VertexShader *vs = new VertexShader(
        "testvs", FileUtils::ReadFile(FileUtils::GetBaseFilePath() +
                                            "../data/materials/vs.glsl"));
    FragmentShader *fs = new FragmentShader(
        "testfs", FileUtils::ReadFile(FileUtils::GetBaseFilePath() +
                                            "../data/materials/fs.glsl"));
    vs->Compile();
    fs->Compile();
    Program* program = new Program(vs, fs);
    program->Link();
    program->RegisterVertexAttribute("position");
    program->RegisterVertexAttribute("normal");
    //program->RegisterAutoUniform("p", Program::PROJECTION_MATRIX);
    //program->RegisterAutoUniform("v", Program::VIEW_MATRIX);
    //program->RegisterAutoUniform("m", Program::WORLD_MATRIX);
    program->RegisterAutoUniform("mvp", Program::MVP_MATRIX);
    mat->set_shader(program);

    md->SetMaterial(mat);
    gfx_->GetRootNode()->AddChild(n);

    n->SetPosition(Vector3(0, 0.5, 0));
    camera_->SetPosition(Vector3(0,0,2));
  }

  virtual void Deinit() {
    delete camera_;
    gfx_->Deinit();
    gfx_.release();
  }

  virtual void Update(double delta) {
    GetSignal("tick")->Send(delta);
    gfx_->Update(delta);
  }

 private:

  FPSCamera* camera_;
  
  Game* game_;
  scoped_ptr<GraphicsContext> gfx_;
};

int main(int argc, char** argv) {
  gnat::FileUtils::SetBaseFilePathFromArgv(argv);
  gnat::SDLPlatformContext *sdl = new gnat::SDLPlatformContext(true, true);
  Game g;
  g.Init(sdl, sdl);
  g.Go(new TestState(), 800, 600, false, true);
  sdl->Deinit();
  delete sdl;
  return 0;
}

