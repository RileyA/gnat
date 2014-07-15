#ifndef gnat_gfx_material_material_H
#define gnat_gfx_material_material_H

#include "gnat.h"

#include "glew.h"
#include "gl.h"

namespace gnat {

class Program;
class GraphicsContext;
class Texture;

class Material {
 public:
  Material(String name);
  virtual ~Material();

  void Use();
  void DoneUsing();

  const String& get_name() const { return name_; }

  void set_shader(Program* p) { shader_program_ = p; }
  Program* shader_program() { return shader_program_; }

  void AddTexture(Texture* texture, String uniform = "");
  void BindTexture(Texture* texture, String uniform);

  static Material* FromFile(GraphicsContext* gfx, String filename);

  void set_draw_group(int group) { draw_group_ = group; }
  void set_depth_write(bool write) { depth_write_ = write; }
  int draw_group() { return draw_group_; }
  void SetDepthCheck(String check);

  void SetBlendMode(String mode);

 private:
  String name_;
  bool using_;

  int draw_group_;
  bool depth_write_;
  GLenum depth_func_;
  GLenum blend_src_;
  GLenum blend_dest_;

  Program* shader_program_;
  Map<String, Texture*> textures_;
};

}

#endif  // gnat_gfx_material_material_H
