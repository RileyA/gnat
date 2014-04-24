#ifndef gnat_gfx_material_material_H
#define gnat_gfx_material_material_H

#include "gnat.h"

namespace gnat {

class Program;

class Material {
 public:
  Material(String name);
  virtual ~Material();

  void Use();
  void DoneUsing();

  const String& get_name() const { return name_; }

  void set_shader(Program* p) { shader_program_ = p; }

  Program* shader_program() { return shader_program_; }

  static Material* FromFile(String filename);

 private:
  String name_;
  bool using_;

  Program* shader_program_;
};

}

#endif  // gnat_gfx_material_material_H
