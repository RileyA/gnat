#ifndef gnat_gfx_util_buffer_object_H 
#define gnat_gfx_util_buffer_object_H

#include "gnat.h"

#include "glew.h"
#include "gl.h"

namespace gnat {

/** A simple wrapper over a GL buffer object */
template <GLenum BUFFER_TYPE> 
class BufferObject {
public:
	
	BufferObject();
	BufferObject(size_t size, GLuint usage) ;
  ~BufferObject();

	bool operator < (const BufferObject<BUFFER_TYPE>& other) const {
		return size() < other.size();
	}

	inline size_t size() const { return size_; }
	void Bind();
	void Unbind();
	bool IsValid() const { return handle_ != 0; }

	void* MapBuffer();
	void* GetMappedBuffer();
	void UnmapBuffer();

	void Destroy();

private:

	size_t size_;
	GLuint handle_;
	bool is_mapped_;
	bool is_bound_;
	// The mapped buffer, if mapped
	void* buffer_;

};

typedef BufferObject<GL_PIXEL_UNPACK_BUFFER> PixelBufferObject;
typedef BufferObject<GL_PIXEL_PACK_BUFFER> PixelBufferObjectPack;
typedef BufferObject<GL_ARRAY_BUFFER> VertexBufferObject;
typedef BufferObject<GL_ELEMENT_ARRAY_BUFFER> IndexBufferObject;

}  // namespace gnat

#endif  // gnat_gfx_util_buffer_object_H
