#include "gnat.h"

#include "gfx/graphics_context.h"
#include "gfx/material/material.h"
#include "gfx/material/program.h"
#include "gfx/material/texture.h"
#include "util/file_utils.h"
#include "util/json.h"

namespace gnat {

Material::Material(String name)
  : name_(name),
    using_(false),
    shader_program_(NULL),
    draw_group_(0),
    depth_write_(true),
    depth_func_(GL_LESS),
    blend_src_(GL_ONE),
    blend_dest_(GL_ONE) {}

Material::~Material() {}

void Material::Use() {
  // For now push/pop everything
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT |
               GL_TEXTURE_BIT);
  if (shader_program_) {
    glUseProgram(shader_program_->handle());
  }

  glDepthMask(depth_write_);
  glDepthFunc(depth_func_);

  if (depth_func_ == GL_ALWAYS)
    glDisable(GL_DEPTH_TEST);
  else
    glEnable(GL_DEPTH_TEST);

  if (blend_src_ != GL_ONE || blend_dest_ != GL_ONE) {
    glEnable(GL_BLEND);
    glBlendFunc(blend_src_, blend_dest_);
  }

  GLenum idx = GL_TEXTURE0;
  for (Map<String, Texture *>::iterator it = textures_.begin(); it != textures_.end();
       ++it)
    it->second->Bind(idx++);
}

void Material::DoneUsing() {
  if (shader_program_) {
    glUseProgram(0);
  }
  glPopAttrib();
}

void Material::AddTexture(Texture* texture, String uniform) {
  textures_[uniform] = texture;
  if (shader_program_ && !uniform.empty()) {
    shader_program_->SetUniform1i(uniform, textures_.size() - 1);
  }
}

void Material::BindTexture(Texture* texture, String uniform) {
  textures_[uniform] = texture;
}

// This is hacky, but I want a working format for Ludum Dare
// TODO clean this up lotses
Material* Material::FromFile(GraphicsContext* gfx, String filename) {
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

    glUseProgram(program->handle());
  
    // Uniforms.
    if (j.Has("uniforms") && j["uniforms"].type() == JsonValue::kArray) {
      for (int i = 0; i < j["uniforms"].size(); ++i) {
        JsonValue& u = j["uniforms"][i];
        if (u.type() != JsonValue::kObject || !u.Has("name") || !u.Has("type"))
          continue;
        String name = u["name"].string();
        String type = u["type"].string();

        // Auto set uniforms.
        if (type == "auto") {
          if (u.Has("auto"))
            program->RegisterAutoUniform(name, u["auto"].string());
        } else {
          float tmpf[4];
          if (type == "int" && u.Has("value") &&
              u["value"].type() == JsonValue::kNumber) {
            program->SetUniform1i(name, u["value"].integer());
          } else if (type == "float" && u.Has("value") &&
                     u["value"].type() == JsonValue::kNumber) {
            program->SetUniform1f(name, u["value"].floating());
          } else if (type == "vec2" && u.Has("value") &&
                     u["value"].type() == JsonValue::kArray &&
                     u["value"].size() == 2) {
            for (int i = 0; i < u["value"].size(); ++i)
              tmpf[i] = u["value"][i].floating();
            program->SetUniform2f(name, tmpf);
          } else if (type == "vec3" && u.Has("value") &&
                     u["value"].type() == JsonValue::kArray &&
                     u["value"].size() == 3) {
            for (int i = 0; i < u["value"].size(); ++i)
              tmpf[i] = u["value"][i].floating();
            program->SetUniform3f(name, tmpf);
          } else if (type == "vec4" && u.Has("value") &&
                     u["value"].type() == JsonValue::kArray &&
                     u["value"].size() == 4) {
            for (int i = 0; i < u["value"].size(); ++i)
              tmpf[i] = u["value"][i].floating();
            program->SetUniform4f(name, tmpf);
          }
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

    // general properties
    if (j.Has("properties") && j["properties"].type() == JsonValue::kObject) {
      JsonValue& p = j["properties"];
      if (p.Has("blend_mode") && p["blend_mode"].type() == JsonValue::kString) {
        m->SetBlendMode(p["blend_mode"].string());
      }
      if (p.Has("draw_group") && p["draw_group"].type() == JsonValue::kNumber) {
        m->set_draw_group(p["draw_group"].integer());
      }
      if (p.Has("depth_write") &&
          p["depth_write"].type() == JsonValue::kBoolean) {
        m->set_depth_write(p["depth_write"]);
      }
      if (p.Has("depth_check") &&
          p["depth_check"].type() == JsonValue::kString) {
        m->SetDepthCheck(p["depth_check"].string());
      }
    }

    if (j.Has("textures") && j["textures"].type() == JsonValue::kArray) {
      for (int i = 0; i < j["textures"].size(); ++i) {
        JsonValue& t = j["textures"][i];
        if (t.type() == JsonValue::kObject) {
          bool alpha = t.Has("alpha") &&
                       t["alpha"].type() == JsonValue::kBoolean && t["alpha"];
          String uniform = "";
          if (t.Has("uniform") && t["uniform"].type() == JsonValue::kString)
            uniform = t["uniform"].string();
          if (t.Has("name"))
            m->AddTexture(gfx->GetTexture(t["name"].string(), alpha), uniform);
          else
            m->AddTexture(0, uniform);
        }
      }
    }

    glUseProgram(0);
  }

  return m;
}

void Material::SetDepthCheck(String check) {
  if (check == "always") {
    depth_func_ = GL_ALWAYS;
  } else if (check == "less_equal") {
    depth_func_ = GL_LEQUAL;
  } else if (check == "less") {
    depth_func_ = GL_LESS;
  } else {
    printf("Unsupported depth func: %s\n", check.c_str());
  }
}

void Material::SetBlendMode(String mode) {
  if (mode == "normal") {
    blend_src_ = GL_ONE;
    blend_dest_ = GL_ZERO;
  } else if (mode == "alpha") {
    blend_src_ = GL_SRC_ALPHA;
    blend_dest_ = GL_ONE_MINUS_SRC_ALPHA;
  } else {
    printf("Unsupported blend mode: %s\n", mode.c_str());
  }
}

}  // namespace gnat
