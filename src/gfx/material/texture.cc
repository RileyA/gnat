#include "gnat.h"

#include "gfx/material/texture.h"
#include "util/file_utils.h"

#include "third_party/png/png.hpp"

namespace gnat {

static GLenum format_to_gl_enum(Texture::Format tf) {
  switch (tf) {
    case Texture::kRGB:
      return GL_RGB;
    case Texture::kRGBA:
      return GL_RGBA;
    case Texture::kDepth:
      return GL_DEPTH_COMPONENT;
  }
  return GL_RGB;
}
//---------------------------------------------------------------------------

static GLenum type_to_gl_enum(Texture::Type type) {
  switch (type) {
    case Texture::kByte:
      return GL_UNSIGNED_BYTE;
    case Texture::kFloat:
      return GL_FLOAT;
  }
  return GL_UNSIGNED_BYTE;
}
//---------------------------------------------------------------------------

static GLenum format_to_size(Texture::Format tf, Texture::Type type) {
  // TODO don't make assumptions, aka actually use |type|...
  switch (tf) {
    case Texture::kRGB:
      return 3;
    case Texture::kRGBA:
      return 4;
    case Texture::kDepth:
      return 4;
  }
  return 4;
}
//---------------------------------------------------------------------------

Texture::Texture(size_t w, size_t h, Texture::Format format, Texture::Type type,
                 GLenum target)
    : w_(w),
      h_(h),
      format_(format),
      type_(type),
      data_(0),
      handle_(0),
      target_(target) {
  glGenTextures(1, &handle_);
  Bind();
  glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(target_, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(target_, GL_TEXTURE_MAX_LEVEL, 0);
  GLenum fmt = format_to_gl_enum(format_);
  GLenum typ = type_to_gl_enum(type_);

  if (target == GL_TEXTURE_CUBE_MAP) {
    // Initialize all the faces for cubemaps.
    for (int i = 0; i < 6; ++i) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, fmt, w_, h_, 0,
                   fmt, typ, 0);
    }
  } else {
    glTexImage2D(target, 0, fmt, w_, h_, 0, fmt, typ, 0);
  }
}
//---------------------------------------------------------------------------

Texture::~Texture() {
  delete data_;
}
//---------------------------------------------------------------------------

void Texture::Write() {
  Bind();
  GLenum fmt = format_to_gl_enum(format_);
  GLenum typ = type_to_gl_enum(type_);
  // TODO handle cubemaps/etc here.
  glTexImage2D(GL_TEXTURE_2D, 0, fmt, w_, h_, 0, fmt, typ, data_);
  glGenerateMipmap(GL_TEXTURE_2D);
}
//---------------------------------------------------------------------------

void Texture::Bind(GLenum idx) {
  glActiveTexture(idx);
  glBindTexture(target_, handle_);
}
//---------------------------------------------------------------------------

unsigned char* Texture::data() {
  if (!data_)
    data_ = new unsigned char[w_ * h_ * format_to_size(format_, type_)];
  return data_;
}
//---------------------------------------------------------------------------

Texture* Texture::Load(String filename, bool alpha) {
  String ext = filename.substr(filename.find_last_of(".") + 1, String::npos);
  if (ext == "png")
    return LoadPNG(filename, alpha);
  else
    return NULL;
}
//---------------------------------------------------------------------------

Texture* Texture::LoadPNG(String filename, bool alpha) {
  try {
    if (alpha) {
      png::image<png::rgba_pixel> image(FileUtils::GetBaseFilePath() +
                                        filename);
      Texture *out = new Texture(image.get_width(), image.get_height(), kRGBA, kByte);
      int w = image.get_width();
      int h = image.get_height();
      for (int i = 0; i < image.get_height(); ++i) {
        for (int j = 0; j < image.get_width(); ++j) {
          out->data()[i * w * 4 + j * 4 + 0] = image[h - i - 1][j].red;
          out->data()[i * w * 4 + j * 4 + 1] = image[h - i - 1][j].green;
          out->data()[i * w * 4 + j * 4 + 2] = image[h - i - 1][j].blue;
          out->data()[i * w * 4 + j * 4 + 3] = image[h - i - 1][j].alpha;
        }
      }
      out->Write();
      return out;
    } else {
      png::image<png::rgb_pixel> image(FileUtils::GetBaseFilePath() +
                                       filename);
      Texture *out = new Texture(image.get_width(), image.get_height(), kRGB, kByte);
      int w = image.get_width();
      int h = image.get_height();
      for (int i = 0; i < image.get_height(); ++i) {
        for (int j = 0; j < image.get_width(); ++j) {
          out->data()[i * w * 3 + j * 3 + 0] = image[h - i - 1][j].red;
          out->data()[i * w * 3 + j * 3 + 1] = image[h - i - 1][j].green;
          out->data()[i * w * 3 + j * 3 + 2] = image[h - i - 1][j].blue;
        }
      }
      out->Write();
      return out;
    }
  } catch(png::std_error e) {
    printf("Failed to load: %s\n", filename.c_str());
    return NULL;
  }
  NOTREACHED();
}
//---------------------------------------------------------------------------

}  // namespace gnat
