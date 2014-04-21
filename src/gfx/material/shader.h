#ifndef gnat_gfx_material_shader_H
#define gnat_gfx_material_shader_H

#include "gnat.h"

#include "glew.h"
#include "gl.h"

namespace gnat {

/** An OpenGL shader program. */
template<GLenum SHADER_TYPE>
class Shader {
 public:
  Shader(String name, String text);
  virtual ~Shader();

  // Is this shader compiled and ready to go?
  bool IsReady() { return ready_; }
  bool Compile();

  String name() { return name_; }
  String text() { return text_; }
  GLuint handle() { return handle_; }

 private:
  String name_;
  String text_;

  GLuint handle_;
  bool ready_;
};

typedef Shader<GL_VERTEX_SHADER> VertexShader;
typedef Shader<GL_FRAGMENT_SHADER> FragmentShader;

}  // namespace gnat

#endif  // gnat_gfx_material_shader_H
