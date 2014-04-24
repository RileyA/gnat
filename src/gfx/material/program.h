#ifndef gnat_gfx_material_program_H
#define gnat_gfx_material_program_H

#include "gnat.h"

#include "gfx/material/shader.h"

namespace gnat {

class Program {
 public:

  // Uniforms that the engine will populate magically.
  enum AutoUniformType {
    WORLD_MATRIX,
    VIEW_MATRIX,
    PROJECTION_MATRIX,
    MVP_MATRIX,
    // Make sure these get added to the string conversion on
    // RegisterAutoUniform(). TODO automate this.
  };

  Program(VertexShader* vs, FragmentShader* fs);
  ~Program();

  bool Link();
  bool IsReady() { return ready_; }

  void RegisterVertexAttribute(String name);
  void RegisterAutoUniform(String name, AutoUniformType uniform);
  void RegisterAutoUniform(String name, String uniform);
  void RegisterUniform(String name);

  Vector<std::pair<String, GLuint> >& GetVertexAttributes() {
    return vertex_attribs_;
  }

  Map<AutoUniformType, GLuint>& GetAutoUniforms() {
    return auto_uniforms_;
  }

  GLuint handle() { return handle_; }

  VertexShader* vs() { return vs_; }
  FragmentShader* fs() { return fs_; }

 private:

  VertexShader* vs_;
  FragmentShader* fs_;
  GLuint handle_;
  bool ready_;

  Map<AutoUniformType, GLuint> auto_uniforms_;
  Map<String, GLuint> uniforms_;

  Vector<std::pair<String, GLuint> > vertex_attribs_;
};

}  // namespace gnat

#endif // gnat_gfx_material_program_H
