#ifndef gnat_gfx_scene_fps_camera_H
#define gnat_gfx_scene_fps_camera_H

#include "gnat.h"

#include "gfx/scene/node.h"
#include "gfx/scene/camera.h"
#include "gfx/math/vector3.h"
#include "event/event_handler.h"

namespace gnat {

class FPSCamera : public EventHandler {
 public:
  FPSCamera();
  virtual ~FPSCamera();

  Node* GetNode() { return &position_; }
  Camera* GetCamera() { return &camera_; }

  void SetMouseSensitivity(Real x, Real y);
  void SetSpeed(Real forward, Real strafe);
  void SetPitchLimits(Real pitch_min, Real pitch_max);
  void SetYawLimits(Real yaw_min, Real yaw_max);

  Vector3 GetDirection();
  Vector3 GetRight();

  void SetPosition(Vector3 pos);
  Vector3 GetPosition();

 private:

  void MouseMoved(const Message& m);
  void KeyDown(const Message& m);
  void KeyUp(const Message& m);
  void Tick(const Message& m);

  bool forward_, backward_, left_, right_;

  Node position_;
  Node yaw_;
  Node roll_;
  Node pitch_;
  Camera camera_;

  Real current_pitch_;
  Real current_yaw_;
  Real pitch_max_, pitch_min_;
  Real yaw_max_, yaw_min_;
  bool yaw_limited_;

  Real sensitivity_x_, sensitivity_y_;
  Real speed_forward_, speed_strafe_;
};

}  // namespace gnat

#endif
