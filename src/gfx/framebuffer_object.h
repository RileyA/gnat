#ifndef gnat_gfx_framebuffer_object_H
#define gnat_gfx_framebuffer_object_H

#include "gnat.h"

#include "glew.h"
#include "gl.h"

namespace gnat {

class Texture;

class FramebufferAttachable {
 public:
  virtual ~FramebufferAttachable() {}
};

// TODO perhaps move this into another file (they are only used with FBOs though
// and have limited functionality in general, so maybe no need...).
class RenderbufferObject : public FramebufferAttachable {
 public:
  RenderbufferObject(GLenum format, GLsizei w, GLsizei h);
  virtual ~RenderbufferObject();

  void Bind();
  void Unbind();

  GLuint handle() { return handle_; }
 private:
  GLuint handle_;
};

class FramebufferObject {
 public:
  FramebufferObject();
  virtual ~FramebufferObject();

  // This gives ownership of |rbo| to this FBO.
  void Attach(GLenum point, RenderbufferObject* rbo);
  void Attach(GLenum point, Texture* tex, GLenum target);
  // This takes ownership of |rbo|.
  FramebufferAttachable* Detach(GLenum point);

  FramebufferAttachable* GetAttachment(GLenum point);

  int GetPixels(int x, int y, size_t w, size_t h, GLenum format, GLenum type,
                unsigned char** out);

  bool IsComplete();

  void Bind();
  void Unbind();

 private:
  bool is_bound_;
  GLuint handle_;
  Map<GLenum, FramebufferAttachable*> attached_buffers_;
};

}  // namespace gnat

#endif
