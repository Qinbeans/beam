#include "beam/objects/color_picker.h"
#include "beam/objects/game_object.h"
#include "raygui.h"
#include <cstring>

namespace beam {

const Rectangle ColorPicker::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void ColorPicker::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  Color prevColor = color;
  GuiColorPicker(getBounds(), nullptr, &color);
  if (memcmp(&color, &prevColor, sizeof(Color)) != 0 && changeCallback) {
    changeCallback(this, manager);
  }
}

void ColorPicker::setColor(Color color) { this->color = color; }

void ColorPicker::setPosition(Vector2 position) { this->position = position; }

void ColorPicker::setSize(Vector2 size) { this->size = size; }

Color ColorPicker::getColor() const { return color; }

Vector2 ColorPicker::getPosition() const { return position; }

Vector2 ColorPicker::getSize() const { return size; }

void ColorPicker::onChange(
    std::function<void(ColorPicker *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
