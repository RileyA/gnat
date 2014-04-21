#include "gnat.h"

#include "shader.h"

namespace gnat {

static void print_errors(GLuint shader) {
    GLint log_length;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    log = static_cast<char*>(malloc(log_length));
    glGetShaderInfoLog(shader, log_length, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
}
//-----------------------------------------------------------------------

template<GLenum SHADER_TYPE>
Shader<SHADER_TYPE>::Shader(String name, String text)
  : name_(name),
    text_(text),
    handle_(0),
    ready_(false) {}
//-----------------------------------------------------------------------

template<GLenum SHADER_TYPE>
Shader<SHADER_TYPE>::~Shader() {
  if (ready_)
    glDeleteShader(handle_);
}
//-----------------------------------------------------------------------

template<GLenum SHADER_TYPE>
bool Shader<SHADER_TYPE>::Compile() {
  handle_ = glCreateShader(SHADER_TYPE);
  const char* str = text_.c_str();
  int length =  text_.size();
  glShaderSource(handle_, 1, (const GLchar**)&str, &length);
  glCompileShader(handle_);
  GLint result;
  glGetShaderiv(handle_, GL_COMPILE_STATUS, &result);
  if (!result) {
    fprintf(stderr, "Failed to compile %s:\n", name_.c_str());
    print_errors(handle_);
    glDeleteShader(handle_);
    ready_ = false;
  }
  ready_ = true;
  return ready_;
}
//-----------------------------------------------------------------------

template class Shader<GL_VERTEX_SHADER>;
template class Shader<GL_FRAGMENT_SHADER>;

}  // namespace gnat
