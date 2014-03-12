#ifndef gnat_gfx_camera_H
#define gnat_gfx_camera_H

#include "gnat.h"

#include "gfx/node.h"
#include "event/event_handler.h"
#include "util/color.h"

namespace gnat {

class Camera : public Node {
 public:
  Camera();
  virtual ~Camera();
};

}  // namespace gnat

#endif  // gnat_gfx_camera_H
