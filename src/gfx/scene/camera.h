#ifndef gnat_gfx_scene_camera_H
#define gnat_gfx_scene_camera_H

#include "gnat.h"

#include "gfx/scene/node.h"
#include "event/event_handler.h"
#include "util/color.h"

namespace gnat {

class Camera : public Node {
 public:
  Camera();
  virtual ~Camera();

  Matrix4 GetProjectionMatrix();
  Matrix4 GetViewMatrix();

  void SetCameraParams(float fov_y, float near_z, float far_z, float aspect_ratio);

 private:

  float fov_y_;
  float near_z_;
  float far_z_;
  float aspect_ratio_;

  Matrix4 projection_;
};

}  // namespace gnat

#endif  // gnat_gfx_camera_H
