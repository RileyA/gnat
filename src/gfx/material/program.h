#ifndef gnat_gfx_material_program_H
#define gnat_gfx_material_program_H

#include "gnat.h"

#include "gfx/material/shader.h"

namespace gnat {

class Program {
 public:
  Program(VertexShader* vs, FragmentShader* fs);
  ~Program();

  bool Link();
  bool IsReady() { return ready_; }

  GLuint handle() { return handle_; }

 private:

  VertexShader* vs_;
  FragmentShader* fs_;
  GLuint handle_;
  bool ready_;

};

}  // namespace gnat

#endif // gnat_gfx_material_program_H
