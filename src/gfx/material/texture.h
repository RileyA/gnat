#ifndef gnat_gfx_material_texture_H
#define gnat_gfx_material_texture_H

#include "gnat.h"

#include "glew.h"
#include "gl.h"

namespace gnat {

class Texture {
 public:
  Texture(size_t w, size_t h, bool alpha);
  virtual ~Texture();

  void Write();
  void Bind(GLenum idx = GL_TEXTURE0);

  int w() { return w_; }
  int h() { return h_; }
  bool alpha() { return alpha_; }
  unsigned char* data() { return data_; }
  GLuint handle() { return handle_; }

  static Texture* Load(String filename, bool alpha);

 private:

  static Texture* LoadPNG(String filename, bool alpha);

  int w_, h_;
  bool alpha_;
  unsigned char* data_;
  GLuint handle_;
};

}

#endif  // 
