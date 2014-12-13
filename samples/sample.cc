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
#include "gfx/scene/voxel/chunk_traits.h"
#include "gfx/scene/voxel/chunk.h"
#include "gfx/material/material.h"
#include "gfx/material/program.h"
#include "gfx/material/shader.h"
#include "gfx/scene/scene_manager.h"
#include "gfx/util/oyster_mesh.h"
#include "util/file_utils.h"
#include "util/timer.h"
#include "game/game.h"
#include "game/game_state.h"

#include "util/scoped_ptr.h"
#include "third_party/oyster/include/Oyster.h"

using namespace gnat;

typedef Chunk<STANDARD> StandardChunk;
typedef ChunkTraits<STANDARD>::Coords Coords;
typedef ChunkTraits<STANDARD> Traits;

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
    game_->input()->GetKeyDownSignal(gnat::K_ESCAPE)
        ->AddListener(game_->GetSlot("shutdown"));
    Signal* tick = CreateSignal("tick");
    camera_->GetSlot("tick")->ListenTo(tick);

    gfx_->SetClearColor(ColorF32(0.12, 0.4, 0.85, 1.0));

    camera_->SetPosition(Vector3(0,0.5,2));
    camera_->SetSpeed(2, 2);

    Material* vox = gfx_->GetMaterial("../data/materials/voxel.material");

    StandardChunk* c = new StandardChunk();
    gfx_->GetRootNode()->AddChild(c);
    for (int i = 0; i < 100; ++i) {
      int x = 1 + rand() % 14;
      int y = 1 + rand() % 14;
      int z = 1 + rand() % 14;
      c->SetVoxel(Coords(x, y, z), 1);
    }
    c->ApplyChanges();
    c->GenerateMeshData();
    c->UpdateMesh();
    c->drawable()->SetMaterial(vox);
    c->SetPosition(Vector3(-8, -8, -8));
  }

  virtual void Deinit() {
    delete camera_;
    gfx_->Deinit();
    gfx_.release();
  }

  virtual void Update(double delta) {
    if (delta > 1.f) {
      delta = 0.f;
    }
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

