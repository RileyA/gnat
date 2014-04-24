#include "gnat.h"

#include "gfx/material/program.h"

namespace gnat {

static void print_errors(GLuint program) {
  GLint log_length;
  char *log;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
  log = static_cast<char*>(malloc(log_length));
  glGetProgramInfoLog(program, log_length, NULL, log);
  fprintf(stderr, "%s", log);
  free(log);
}
//-----------------------------------------------------------------------

Program::Program(VertexShader* vs, FragmentShader* fs)
  : vs_(vs),
    fs_(fs),
    handle_(0),
    ready_(false) {}
//-----------------------------------------------------------------------

Program::~Program() {
  if (ready_)
    glDeleteProgram(handle_);
  delete vs_;
  delete fs_;
}
//-----------------------------------------------------------------------

bool Program::Link() {
  DCHECK(vs_ && fs_ && vs_->IsReady() && fs_->IsReady());
  handle_ = glCreateProgram();
  glAttachShader(handle_, vs_->handle());
  glAttachShader(handle_, fs_->handle());
  glLinkProgram(handle_);

  GLint result;
  glGetProgramiv(handle_, GL_LINK_STATUS, &result);
  if (!result) {
    fprintf(stderr, "Failed to link shader program:\n");
    print_errors(handle_);
    glDeleteProgram(handle_);
    ready_ = false;
  } else {
    ready_ = true;
  }

  return ready_;
}
//-----------------------------------------------------------------------

void Program::RegisterVertexAttribute(String name) {
  DCHECK(ready_);
  GLuint addr = glGetAttribLocation(handle_, name.c_str());
  DCHECK(glGetError() == GL_NO_ERROR);
  vertex_attribs_.push_back(std::make_pair(name, addr));
}
//-----------------------------------------------------------------------

void Program::RegisterAutoUniform(String name, AutoUniformType uniform) {
  DCHECK(ready_);
  GLuint addr = glGetUniformLocation(handle_, name.c_str());
  DCHECK(glGetError() == GL_NO_ERROR);
  uniforms_[name] = addr;
  auto_uniforms_[uniform] = addr;
}
//-----------------------------------------------------------------------

void Program::RegisterAutoUniform(String name, String uniform) {
  AutoUniformType type;
  if (uniform == "MVP_MATRIX")
    type = MVP_MATRIX;
  else if (uniform == "WORLD_MATRIX")
    type = WORLD_MATRIX;
  else if (uniform == "VIEW_MATRIX")
    type = VIEW_MATRIX;
  else if (uniform == "PROJECTION_MATRIX")
    type = PROJECTION_MATRIX;
  else
    return;
  RegisterAutoUniform(name, type);
}
//-----------------------------------------------------------------------

void Program::RegisterUniform(String name) {
  DCHECK(ready_);
  GLuint addr = glGetUniformLocation(handle_, name.c_str());
  DCHECK(glGetError() == GL_NO_ERROR);
  uniforms_[name] = addr;
}
//-----------------------------------------------------------------------

}  // namespace gnat
