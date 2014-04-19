#ifndef gnat_gfx_material_material_H
#define gnat_gfx_material_material_H

#include "gnat.h"

namespace gnat {

class Material {
 public:
  Material(String name);
  virtual ~Material();

  const String& get_name() const { return name_; }

 private:
  String name_;
};

}

#endif  // gnat_gfx_material_material_H
