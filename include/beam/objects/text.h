

// Text.hpp
#pragma once

#include "beam/objects/game_object.h"

namespace beam {

class Text : public GameObject {
private:
  std::string content;
  int fontSize;
  Color color;

public:
  Text(const std::string &text, float x, float y, int size = 20);

  void draw() override;

  void setText(const std::string &text);
  void setColor(Color newColor);
  void setFontSize(int size);

  const std::string &getText() const;
  Color getColor() const;
  int getFontSize() const;
};

} // namespace beam
