#include "beam/objects/color_bar_hue.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle ColorBarHue::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void ColorBarHue::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  float prevHue = hue;
  GuiColorBarHue(getBounds(), nullptr, &hue);
  if (hue != prevHue && changeCallback) {
    changeCallback(this, manager);
  }
}

void ColorBarHue::setHue(float hue) { this->hue = hue; }

void ColorBarHue::setPosition(Vector2 position) { this->position = position; }

void ColorBarHue::setSize(Vector2 size) { this->size = size; }

float ColorBarHue::getHue() const { return hue; }

Vector2 ColorBarHue::getPosition() const { return position; }

Vector2 ColorBarHue::getSize() const { return size; }

void ColorBarHue::onChange(
    std::function<void(ColorBarHue *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
