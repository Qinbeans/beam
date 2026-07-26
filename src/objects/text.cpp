#include "beam/objects/text.h"
#include "beam/objects/game_object.h"

namespace beam {

Text::Text(const std::string &text, float x, float y, const std::string &fontName, float fontSize, float fontSpacing)
    : GameObject("Text"), content(text), fontSize(fontSize), fontSpacing(fontSpacing), fontName(fontName), color(BLACK) {
  position = {x, y};
}

void Text::draw(SharedManager manager) {
  Font font;
  if (manager->hasAsset(fontName)) {
    font = manager->getAsset<Font>(fontName);
  } else {
    font = GetFontDefault();
  }
  if (active) {
    DrawTextEx(font, content.c_str(), position, fontSize, fontSpacing, color);
  }
  GameObject::draw(manager);
}

void Text::setText(const std::string &text) { content = text; }

void Text::setColor(Color newColor) { color = newColor; }

void Text::setFontName(const std::string &fontName) { this->fontName = fontName; }

void Text::setFontSize(float size) { fontSize = size; }

void Text::setFontSpacing(float spacing) { fontSpacing = spacing; }

const std::string &Text::getText() const { return content; }

Color Text::getColor() const { return color; }

float Text::getFontSize() const { return fontSize; }

} // namespace beam