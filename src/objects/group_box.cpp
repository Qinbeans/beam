#include "beam/objects/group_box.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle GroupBox::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void GroupBox::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  int prevLineColor = GuiGetStyle(DEFAULT, LINE_COLOR);
  GuiSetStyle(DEFAULT, LINE_COLOR, ColorToInt(color));

  GuiGroupBox(getBounds(), text.empty() ? nullptr : text.c_str());

  GuiSetStyle(DEFAULT, LINE_COLOR, prevLineColor);
}

void GroupBox::setText(const std::string &text) { this->text = text; }

void GroupBox::setPosition(Vector2 position) { this->position = position; }

void GroupBox::setSize(Vector2 size) { this->size = size; }

void GroupBox::setColor(Color color) { this->color = color; }

const std::string &GroupBox::getText() const { return text; }

Vector2 GroupBox::getPosition() const { return position; }

Vector2 GroupBox::getSize() const { return size; }

Color GroupBox::getColor() const { return color; }

} // namespace beam
