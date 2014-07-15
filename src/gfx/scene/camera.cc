#include "gfx/scene/camera.h"

namespace gnat {

Camera::Camera() {
  SetCameraParams(40.0, 0.01, 100.0, 1.333);
}

//---------------------------------------------------------------------------

Camera::~Camera() {}
//---------------------------------------------------------------------------

Matrix4 Camera::GetProjectionMatrix() {
  return projection_;
}
//---------------------------------------------------------------------------

Matrix4 Camera::GetViewMatrix() {
  return GetTransform().Inverse();
}
//---------------------------------------------------------------------------

void Camera::SetCameraParams(float fov_y, float near_z, float far_z,
                             float aspect_ratio) {
  fov_y_ = fov_y;
  near_z_ = near_z;
  far_z_ = far_z;
  aspect_ratio_ = aspect_ratio;
  float f = (1.0 / tan((fov_y_ * (3.141592 / 180.0)) / 2.0));
  projection_ = Matrix4(f / aspect_ratio_, 0, 0, 0,
                 0, f, 0, 0,
                 0, 0, (near_z_ + far_z_) / (near_z_ - far_z_),
                    (2.0 * near_z_ * far_z_) / (near_z_ - far_z_), 
                 0, 0, -1, 0);
}
//---------------------------------------------------------------------------

}  // namespace gnat
