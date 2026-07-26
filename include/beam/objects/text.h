// Text.hpp
#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"

namespace beam {

class Text : public GameObject {
private:
  std::string content;
  float fontSize;
  float fontSpacing;
  std::string fontName;
  Color color;

public:
  Text(const std::string &text, float x, float y, const std::string &fontName = "default", float fontSize = 20.0f, float fontSpacing = 1.0f);

  void draw(SharedManager) override;

  void setText(const std::string &text);
  void setColor(Color newColor);
  void setFontName(const std::string &fontName);
  void setFontSize(float size);
  void setFontSpacing(float spacing);

  const std::string &getText() const;
  Color getColor() const;
  float getFontSize() const;
};

} // namespace beam
