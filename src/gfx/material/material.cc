#include "gnat.h"

#include "gfx/material/material.h"
#include "gfx/material/program.h"
#include "util/file_utils.h"
#include "util/json.h"

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

// This is hacky, but I want a working format for Ludum Dare
// TODO clean this up lotses
Material* Material::FromFile(String filename) {
  String text;
  if (!FileUtils::ReadFile(FileUtils::GetBaseFilePath() + filename, &text)) {
    return NULL;
  }
  scoped_ptr<JsonValue> json = JsonValue::Parse(text);
  if (json.IsNull()) {
    return NULL;
  }
  DCHECK((*json)["name"]);
  Material* m = new Material((*json)["name"].string());

  JsonValue& j = (*json)["program"];
  if (j && j.Has("vertex_program") && j.Has("fragment_program")) {
    String path = j["vertex_program"].string();
    String tmp;
    if (!FileUtils::ReadFile(FileUtils::GetBaseFilePath() + path, &tmp))
      return m;
    VertexShader* vs = new VertexShader(path, tmp);
    if (!vs->Compile()) {
      delete vs;
      return m;
    }
    path = j["fragment_program"].string();
    tmp.clear();
    if (!FileUtils::ReadFile(FileUtils::GetBaseFilePath() + path, &tmp))
      return m;
    FragmentShader* fs = new FragmentShader(path, tmp);
    if (!fs->Compile()) {
      delete vs;
      delete fs;
      return m;
    }
    Program* program = new Program(vs, fs);
    if (!program->Link()) {
      delete program;
      return m;
    }
    m->set_shader(program);

    // Uniforms.
    if (j.Has("uniforms") && j["uniforms"].type() == JsonValue::kArray) {
      for (int i = 0; i < j["uniforms"].size(); ++i) {
        JsonValue& u = j["uniforms"][i];
        if (u.type() != JsonValue::kObject || !u.Has("name") || !u.Has("type"))
          continue;
        String name = u["name"].string();
        String type = u["type"].string();
        if (type == "auto") {
          if (u.Has("auto"))
            program->RegisterAutoUniform("mvp", u["auto"].string());
        } else {
          // TODO
        }
      }
    }
    
    // Vertex attributes.
    if (j.Has("attributes") && j["attributes"].type() == JsonValue::kArray) {
      for (int i = 0; i < j["attributes"].size(); ++i) {
        JsonValue& a = j["attributes"][i];
        if (a.type() == JsonValue::kString)
          program->RegisterVertexAttribute(a.string());
      }
    }
  }

  return m;
}

}  // namespace gnat
