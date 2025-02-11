#pragma once

#include <string>

#include <freetype/freetype.h>
#include <freetype/ftlcdfil.h>

namespace freetype {

struct Glyph {
  FT_GlyphSlotRec * slot = nullptr;
  std::size_t height = 0;
  std::size_t left = 0;
  std::size_t top = 0;
  std::size_t width = 0;
  void * pixels = nullptr;
};

//RESEARCH: how glyphs are associated to faces?
struct Face {
  ~Face();

  Face() = default;
  Face(const Face &) = delete;
  Face(Face && other) {
    std::swap(face_, other.face_);
  }
  Face & operator = (const Face &) = delete;
  Face & operator = (Face && other) {
    std::swap(face_, other.face_);
    return *this;
  }

  std::size_t lineHeight() const {
    return lineHeight_;
  }

  std::size_t glyphWidth() const {
    return glyphWidth_;
  }

  Glyph glyph(const char) const;

// private:
  FT_Face face_ = nullptr;
  std::size_t lineHeight_ = 0;
  std::size_t glyphWidth_ = 0;
};

//TODO: this is most likely a singleton
struct Library {
  ~Library();
  Library();

  Face load(const std::string & filename, const std::size_t size = 12, const std::size_t dpi = 150);

private:
  FT_Library library_ = nullptr;
};

} // end of freetype namespace
