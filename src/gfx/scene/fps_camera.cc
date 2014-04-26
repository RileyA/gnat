#include "gnat.h"

#include "gfx/scene/fps_camera.h"
#include "event/message.h"
#include "platform/keycodes.h"

namespace gnat {

FPSCamera::FPSCamera()
    : forward_(false),
      backward_(false),
      left_(false),
      right_(false),
      current_pitch_(0.0),
      pitch_min_(-80.0 * 3.141592 / 180.0),
      pitch_max_(80.0 * 3.141592 / 180.0),
      yaw_min_(-80.0 * 3.141592 / 180.0),
      yaw_max_(80.0 * 3.141592 / 180.0),
      sensitivity_x_(0.001),
      sensitivity_y_(0.001),
      speed_forward_(0.5),
      speed_strafe_(0.5) {
  CreateSlot("mouse_moved", this, &FPSCamera::MouseMoved);
  CreateSlot("key_down", this, &FPSCamera::KeyDown);
  CreateSlot("key_up", this, &FPSCamera::KeyUp);
  CreateSlot("tick", this, &FPSCamera::Tick);
  position_.AddChild(&yaw_);
  yaw_.AddChild(&pitch_);
  pitch_.AddChild(&roll_);
  roll_.AddChild(&camera_);
}
//---------------------------------------------------------------------------

FPSCamera::~FPSCamera() {}
//---------------------------------------------------------------------------

void FPSCamera::SetMouseSensitivity(Real x, Real y) {
  sensitivity_x_ = x;
  sensitivity_y_ = y;
}
//---------------------------------------------------------------------------

void FPSCamera::SetSpeed(Real forward, Real strafe) {
  speed_forward_ = forward;
  speed_strafe_ = strafe;
}
//---------------------------------------------------------------------------

void FPSCamera::SetPitchLimits(Real pitch_min, Real pitch_max) {
  pitch_min_ = pitch_min * 3.141592 / 180.0;
  pitch_max_ = pitch_max * 3.141592 / 180.0;
}
//---------------------------------------------------------------------------

void FPSCamera::SetYawLimits(Real yaw_min, Real yaw_max) {
  yaw_min_ = yaw_min * 3.141592 / 180.0;
  yaw_max_ = yaw_max * 3.141592 / 180.0;
}
//---------------------------------------------------------------------------

Vector3 FPSCamera::GetDirection() {
  return camera_.GetTransform().ExtractQuaternion() * Vector3::NEGATIVE_UNIT_Z;
}
//---------------------------------------------------------------------------

Vector3 FPSCamera::GetRight() {
  return camera_.GetTransform().ExtractQuaternion() * Vector3::UNIT_X;
}
//---------------------------------------------------------------------------

void FPSCamera::SetPosition(Vector3 pos) {
  position_.SetPosition(pos);
}
//---------------------------------------------------------------------------

Vector3 FPSCamera::GetPosition() {
  return position_.GetPosition();
}
//---------------------------------------------------------------------------

void FPSCamera::MouseMoved(const Message& m) {
  gnat::Vector3 move = gnat::message_cast<gnat::Vector3>(m);

  float yaw = sensitivity_x_ * -move.x;
  float pitch = sensitivity_y_ * -move.y;

  if (current_yaw_ + yaw > yaw_max_) {
    yaw = yaw_max_ - current_yaw_;
    current_yaw_ = yaw_max_;
  } else if (current_yaw_ + yaw < yaw_min_) {
    yaw = yaw_min_ - current_yaw_;
    current_yaw_ = yaw_min_;
  } else {
    current_yaw_ += yaw;
  }

  if (current_pitch_ + pitch > pitch_max_) {
    pitch = pitch_max_ - current_pitch_;
    current_pitch_ = pitch_max_;
  } else if (current_pitch_ + pitch < pitch_min_) {
    pitch = pitch_min_ - current_pitch_;
    current_pitch_ = pitch_min_;
  } else {
    current_pitch_ += pitch;
  }

  gnat::Quaternion q = gnat::Quaternion::IDENTITY;
  q.FromAngleAxis(current_pitch_, gnat::Vector3(1, 0, 0));
  pitch_.SetOrientation(q);
  q.FromAngleAxis(current_yaw_, gnat::Vector3(0, 1, 0));
  yaw_.SetOrientation(q);
}
//---------------------------------------------------------------------------

void FPSCamera::KeyDown(const Message& m) {
  switch (message_cast<KeyCode>(m)) {
    case K_W:
      forward_ = true;
      break;
    case K_A:
      left_ = true;
      break;
    case K_S:
      backward_ = true;
      break;
    case K_D:
      right_ = true;
      break;
  }
}
//---------------------------------------------------------------------------

void FPSCamera::KeyUp(const Message& m) {
  switch (message_cast<KeyCode>(m)) {
    case K_W:
      forward_ = false;
      break;
    case K_A:
      left_ = false;
      break;
    case K_S:
      backward_ = false;
      break;
    case K_D:
      right_ = false;
      break;
  }
}
//---------------------------------------------------------------------------

void FPSCamera::Tick(const Message& m) {
  double delta = message_cast<double>(m);
  if (!forward_ && !backward_ && !right_ && !left_)
    return;
  Vector3 motion = Vector3::ZERO;
  motion += (GetDirection() * (forward_ * 1 + backward_ * -1)) * speed_forward_;
  motion += (GetRight() * (right_ * 1 + left_ * -1)) * speed_strafe_;
  if (motion != Vector3::ZERO)
    SetPosition(GetPosition() + motion * delta);
}
//---------------------------------------------------------------------------

};
