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

  void RegisterVertexAttribute(String name);

  Vector<std::pair<String, GLuint> >& GetVertexAttributes() {
    return vertex_attribs_;
  }

  GLuint handle() { return handle_; }

  VertexShader* vs() { return vs_; }
  FragmentShader* fs() { return fs_; }

 private:

  VertexShader* vs_;
  FragmentShader* fs_;
  GLuint handle_;
  bool ready_;

  Vector<std::pair<String, GLuint> > vertex_attribs_;
};

}  // namespace gnat

#endif // gnat_gfx_material_program_H
