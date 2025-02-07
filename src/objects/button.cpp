#include "beam/objects/button.h"
#include "beam/core/manager.h"
#include "raylib.h"

namespace beam {
void Button::draw(SharedManager manager) {
  Font font;
  if (manager->hasAsset("buttonFont")) {
    font = manager->getAsset<Font>("buttonFont");
  } else {
    font = GetFontDefault();
  }
  const float width = size.x + padding.left + padding.right;
  const float height = size.y + padding.top + padding.bottom;
  const Vector2 text_position = {this->position.x + padding.left,
                                 this->position.y + padding.top};

  if (hovered) {
    DrawRectangle(position.x, position.y, width, height, bgHover);
    DrawText(text.c_str(), text_position.x, text_position.y, fontSize, fgHover);
  } else {
    DrawRectangle(position.x, position.y, width, height, bg);
    DrawText(text.c_str(), text_position.x, text_position.y, fontSize, fg);
  }
}

const Rectangle Button::getBounds() const {
  return {position.x, position.y,
          static_cast<float>(size.x + padding.left + padding.right),
          static_cast<float>(size.y + padding.top + padding.bottom)};
}

bool Button::isHovered() const {
  return CheckCollisionPointRec(GetMousePosition(), getBounds());
}

bool Button::isClicked() const {
  return isHovered() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void Button::onUpdate(std::function<void(Button *, SharedManager)> callback) {
  updateCallback = callback;
}

void Button::onHover(std::function<void(Button *, SharedManager)> callback) {
  hoverCallback = callback;
}

void Button::onClick(std::function<void(Button *, SharedManager)> callback) {
  clickCallback = callback;
}

void Button::update(float, SharedManager manager) {
  if (updateCallback) {
    updateCallback(this, manager);
  }

  if (isHovered()) {
    if (hoverCallback) {
      hoverCallback(this, manager);
    }
  }

  if (isClicked()) {
    if (clickCallback) {
      clickCallback(this, manager);
    }
  }
}

void Button::setText(const std::string &text) { this->text = text; }

void Button::setFontSize(float fontSize) { this->fontSize = fontSize; }

void Button::setPosition(Vector2 position) { this->position = position; }

void Button::setSize(Vector2 size) { this->size = size; }

void Button::setPadding(Padding padding) { this->padding = padding; }

void Button::setBgColor(Color color) { bg = color; }

void Button::setFgColor(Color color) { fg = color; }

void Button::setBgHoverColor(Color color) { bgHover = color; }

void Button::setFgHoverColor(Color color) { fgHover = color; }

} // namespace beam