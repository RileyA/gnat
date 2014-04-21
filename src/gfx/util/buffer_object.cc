#include "buffer_object.h"

#include "gl.h"

namespace gnat {

template <GLenum BUFFER_TYPE>
BufferObject<BUFFER_TYPE>::BufferObject()
  : size_(0),
    handle_(0),
    is_bound_(false),
    is_mapped_(false),
    buffer_(0) {}
//-----------------------------------------------------------------------

template <GLenum BUFFER_TYPE>
BufferObject<BUFFER_TYPE>::BufferObject(size_t size, GLuint usage) 
  : size_(size),
    is_bound_(false),
    is_mapped_(false) {
  glGenBuffers(1, &handle_);
  Bind();
  glBufferData(BUFFER_TYPE, size_, 0, usage);
  Unbind();
}
//-----------------------------------------------------------------------

template <GLenum BUFFER_TYPE>
BufferObject<BUFFER_TYPE>::~BufferObject() {}
//-----------------------------------------------------------------------

template <GLenum BUFFER_TYPE>
void BufferObject<BUFFER_TYPE>::Bind() {
  DCHECK(IsValid());
  if (!is_bound_) {
    glBindBuffer(BUFFER_TYPE, handle_);
    is_bound_ = true;
  }
}
//-----------------------------------------------------------------------

template <GLenum BUFFER_TYPE>
void BufferObject<BUFFER_TYPE>::Unbind() {
  DCHECK(IsValid());
  if (is_bound_) {
    glBindBuffer(BUFFER_TYPE, 0);
    is_bound_ = false;
  }
}
//-----------------------------------------------------------------------

template <GLenum BUFFER_TYPE>
void* BufferObject<BUFFER_TYPE>::MapBuffer() {
  DCHECK(IsValid());
  if (!is_mapped_)	{
    // if already bound, keep it that way, otherwise bind and unbind
    bool wasBound = is_bound_;
    if (!wasBound) Bind();
    buffer_ = glMapBuffer(BUFFER_TYPE, GL_WRITE_ONLY);
    is_mapped_ = true;
    if (!wasBound) Unbind();
  }
  return buffer_;
}
//-----------------------------------------------------------------------

template <GLenum BUFFER_TYPE>
inline void* BufferObject<BUFFER_TYPE>::GetMappedBuffer() {
  DCHECK(IsValid());
  DCHECK(is_mapped_);
  DCHECK(buffer_);
  return buffer_;
}
//-----------------------------------------------------------------------

template <GLenum BUFFER_TYPE>
void BufferObject<BUFFER_TYPE>::UnmapBuffer() {
  DCHECK(IsValid());
  if (is_mapped_) {
    // if already bound, keep it that way, otherwise bind and unbind
    bool wasBound = is_bound_;
    Bind();
    glUnmapBuffer(BUFFER_TYPE);
    is_mapped_ = false;
    buffer_ = 0;
    Unbind();
  }
}
//-----------------------------------------------------------------------

template <GLenum BUFFER_TYPE>
void BufferObject<BUFFER_TYPE>::Destroy() {
  DCHECK(IsValid());
  if (is_mapped_) UnmapBuffer();
  if (is_bound_) Unbind();
  glDeleteBuffers(1, &handle_);
  handle_ = 0;
}
//-----------------------------------------------------------------------

// Magic declarations to hint to the compiler that we'll use these
// specializations, we'll hopefully get linker errors if we try to use
// anything else.
template class BufferObject<GL_PIXEL_UNPACK_BUFFER>;
template class BufferObject<GL_PIXEL_PACK_BUFFER>;
template class BufferObject<GL_ARRAY_BUFFER>;
template class BufferObject<GL_ELEMENT_ARRAY_BUFFER>;

}  // namespace gnat
