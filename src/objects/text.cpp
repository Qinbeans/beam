#include "beam/objects/text.h"
#include "beam/objects/game_object.h"

namespace beam {

Text::Text(const std::string &text, float x, float y, int size)
    : GameObject("Text"), content(text), fontSize(size), color(BLACK) {
  position = {x, y};
}

void Text::draw(SharedManager managers) {
  if (active) {
    DrawText(content.c_str(), static_cast<int>(position.x),
             static_cast<int>(position.y), fontSize, color);
  }
  GameObject::draw(managers);
}

void Text::setText(const std::string &text) { content = text; }

void Text::setColor(Color newColor) { color = newColor; }

void Text::setFontSize(int size) { fontSize = size; }

const std::string &Text::getText() const { return content; }

Color Text::getColor() const { return color; }

int Text::getFontSize() const { return fontSize; }

} // namespace beam