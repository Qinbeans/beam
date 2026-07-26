#include "beam/objects/line.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle Line::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void Line::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  int prevLineColor = GuiGetStyle(DEFAULT, LINE_COLOR);
  GuiSetStyle(DEFAULT, LINE_COLOR, ColorToInt(color));

  GuiLine(getBounds(), text.empty() ? nullptr : text.c_str());

  GuiSetStyle(DEFAULT, LINE_COLOR, prevLineColor);
}

void Line::setText(const std::string &text) { this->text = text; }

void Line::setPosition(Vector2 position) { this->position = position; }

void Line::setSize(Vector2 size) { this->size = size; }

void Line::setColor(Color color) { this->color = color; }

const std::string &Line::getText() const { return text; }

Vector2 Line::getPosition() const { return position; }

Vector2 Line::getSize() const { return size; }

Color Line::getColor() const { return color; }

} // namespace beam
