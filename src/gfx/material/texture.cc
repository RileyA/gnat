#include "gnat.h"

#include "gfx/material/texture.h"
#include "util/file_utils.h"

#include "third_party/png/png.hpp"

namespace gnat {

Texture::Texture(size_t w, size_t h, bool alpha)
  : w_(w),
    h_(h),
    alpha_(alpha),
    handle_(0) {
  data_ = new unsigned char[w * h * (alpha ? 4 : 3)];
  glGenTextures(1, &handle_);
}
//---------------------------------------------------------------------------

Texture::~Texture() {
  delete data_;
}
//---------------------------------------------------------------------------

void Texture::Write() {
  Bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, alpha_ ? GL_RGBA : GL_RGB, w_, h_, 0,
               alpha_ ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data_);
  glGenerateMipmap(GL_TEXTURE_2D);
}
//---------------------------------------------------------------------------

void Texture::Bind(GLenum idx) {
  glActiveTexture(idx);
  glBindTexture(GL_TEXTURE_2D, handle_);
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
      Texture *out = new Texture(image.get_width(), image.get_height(), true);
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
      Texture *out = new Texture(image.get_width(), image.get_height(), false);
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
