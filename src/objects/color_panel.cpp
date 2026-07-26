#include "beam/objects/color_panel.h"
#include "beam/objects/game_object.h"
#include "raygui.h"
#include <cstring>

namespace beam {

const Rectangle ColorPanel::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void ColorPanel::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  Color prevColor = color;
  GuiColorPanel(getBounds(), nullptr, &color);
  if (memcmp(&color, &prevColor, sizeof(Color)) != 0 && changeCallback) {
    changeCallback(this, manager);
  }
}

void ColorPanel::setColor(Color color) { this->color = color; }

void ColorPanel::setPosition(Vector2 position) { this->position = position; }

void ColorPanel::setSize(Vector2 size) { this->size = size; }

Color ColorPanel::getColor() const { return color; }

Vector2 ColorPanel::getPosition() const { return position; }

Vector2 ColorPanel::getSize() const { return size; }

void ColorPanel::onChange(
    std::function<void(ColorPanel *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
