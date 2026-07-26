#include "beam/objects/color_bar_alpha.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle ColorBarAlpha::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void ColorBarAlpha::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  float prevAlpha = alpha;
  GuiColorBarAlpha(getBounds(), nullptr, &alpha);
  if (alpha != prevAlpha && changeCallback) {
    changeCallback(this, manager);
  }
}

void ColorBarAlpha::setAlpha(float alpha) { this->alpha = alpha; }

void ColorBarAlpha::setPosition(Vector2 position) {
  this->position = position;
}

void ColorBarAlpha::setSize(Vector2 size) { this->size = size; }

float ColorBarAlpha::getAlpha() const { return alpha; }

Vector2 ColorBarAlpha::getPosition() const { return position; }

Vector2 ColorBarAlpha::getSize() const { return size; }

void ColorBarAlpha::onChange(
    std::function<void(ColorBarAlpha *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
