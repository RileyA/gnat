#ifndef gnat_gfx_material_texture_H
#define gnat_gfx_material_texture_H

#include "gnat.h"

#include "glew.h"
#include "gl.h"

#include "gfx/framebuffer_object.h"

namespace gnat {

class Texture : public FramebufferAttachable {
 public:

  enum Format {
    kRGB,
    kRGBA,
    kDepth,
  };

  enum Type {
    kByte,
    kFloat,
  };

  Texture(size_t w, size_t h, Format format, Type type,
          GLenum target = GL_TEXTURE_2D);
  virtual ~Texture();

  /** Write our image to the GPU from backing store. */
  void Write();

  /** Read image from GPU to our CPU backing store. */
  //TODO void Read();

  void Bind(GLenum idx = GL_TEXTURE0);

  int w() { return w_; }
  int h() { return h_; }
  unsigned char* data();
  GLenum target() { return target_; }
  GLuint handle() { return handle_; }

  static Texture* Load(String filename, bool alpha);

 private:

  static Texture* LoadPNG(String filename, bool alpha);

  int w_, h_;
  Format format_;
  Type type_;
  GLenum target_;

  // CPU backing store. Lazily initialized.
  unsigned char* data_;
  GLuint handle_;
};

}

#endif  // 
