#include "gnat.h"
#include "gfx/framebuffer_object.h"
#include "gfx/material/texture.h"

namespace gnat {

RenderbufferObject::RenderbufferObject(GLenum format, GLsizei w, GLsizei h) {
  handle_ = 0;
  glGenRenderbuffers(1, &handle_);
  Bind();
  glRenderbufferStorage(GL_RENDERBUFFER, format, w, h);
  Unbind();
}
//-------------------------------------------------------------------------

RenderbufferObject::~RenderbufferObject() {
  glDeleteRenderbuffers(1, &handle_);
}
//-------------------------------------------------------------------------

void RenderbufferObject::Bind() {
  glBindRenderbuffer(GL_RENDERBUFFER, handle_);
}
//-------------------------------------------------------------------------

void RenderbufferObject::Unbind() {
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

FramebufferObject::FramebufferObject()
  : handle_(0) {
  glGenFramebuffers(1, &handle_);
  is_bound_ = false;
}
//-------------------------------------------------------------------------

FramebufferObject::~FramebufferObject() {
  glDeleteFramebuffers(1, &handle_);
  for (Map<GLenum, FramebufferAttachable*>::iterator it =
           attached_buffers_.begin();
       it != attached_buffers_.end(); ++it)
    delete it->second;
}
//-------------------------------------------------------------------------

void FramebufferObject::Attach(GLenum point, RenderbufferObject* rbo) {
  DCHECK(is_bound_);
  if (attached_buffers_.count(point)) {
    delete attached_buffers_[point];
  }
  attached_buffers_[point] = rbo;

  rbo->Bind();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, point,
                            GL_RENDERBUFFER, rbo->handle());
  rbo->Unbind();
}
//-------------------------------------------------------------------------

void FramebufferObject::Attach(GLenum point, Texture* texture, GLenum target) {
  DCHECK(is_bound_);
  if (attached_buffers_.count(point)) {
    delete attached_buffers_[point];
  }
  attached_buffers_[point] = texture;

  texture->Bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         point,
                         target,
                         texture->handle(),
                         0);
}
//-------------------------------------------------------------------------

FramebufferAttachable* FramebufferObject::Detach(GLenum point) {
  if (!attached_buffers_.count(point))
    return NULL;
  if (dynamic_cast<RenderbufferObject*>(attached_buffers_[point])) {
    RenderbufferObject* out = (RenderbufferObject*)attached_buffers_[point];
    attached_buffers_.erase(attached_buffers_.find(point));
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, point, GL_RENDERBUFFER, 0);
    return out;
  } else if (dynamic_cast<Texture*>(attached_buffers_[point])) {
    Texture* out = (Texture*)attached_buffers_[point];
    attached_buffers_.erase(attached_buffers_.find(point));
    glFramebufferTexture2D(GL_FRAMEBUFFER, point, GL_TEXTURE_2D, 0, 0);
    return out;
  }
  NOTREACHED();
  return NULL;
}
//-------------------------------------------------------------------------

FramebufferAttachable* FramebufferObject::GetAttachment(GLenum point) {
  return attached_buffers_.count(point) ? attached_buffers_[point] : NULL;
}
//-------------------------------------------------------------------------

int FramebufferObject::GetPixels(int x, int y, size_t w, size_t h,
                                 GLenum format, GLenum type,
                                 unsigned char** out) {
  // TODO: handle this differently if we have textures bound.
  DCHECK(is_bound_);
  DCHECK(type == GL_UNSIGNED_BYTE || type == GL_FLOAT ||
         type == GL_UNSIGNED_INT);
  DCHECK(format == GL_RGB || GL_DEPTH_COMPONENT || format == GL_RGBA);
  size_t bufsize = (w - x) * (h - y);
  if (type == GL_UNSIGNED_BYTE) bufsize *= format == GL_RGBA ? 4 : 3;
  if (type == GL_UNSIGNED_INT) bufsize *= 4;
  if (type == GL_FLOAT) bufsize *= 4;
  *out = new unsigned char[bufsize];
  glReadPixels(x, y, w, h, format, type, *out);
  return bufsize;
}
//-------------------------------------------------------------------------

bool FramebufferObject::IsComplete() {
  DCHECK(is_bound_);
  return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}
//-------------------------------------------------------------------------

void FramebufferObject::Bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, handle_);
  is_bound_ = true;
}
//-------------------------------------------------------------------------

void FramebufferObject::Unbind() {
  // 0 is the window-system provided framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  is_bound_ = false;
}
//-------------------------------------------------------------------------

}  // namespace gnat
