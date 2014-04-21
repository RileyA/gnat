#include "gnat.h"

#include "gfx/material/material.h"
#include "gfx/material/program.h"

namespace gnat {

Material::Material(String name)
  : name_(name),
    using_(false),
    shader_program_(NULL) {}

Material::~Material() {}

void Material::Use() {
  if (shader_program_) {
    glUseProgram(shader_program_->handle());
  }
}

void Material::DoneUsing() {
  if (shader_program_) {
    glUseProgram(0);
  }
}

}  // namespace gnat
