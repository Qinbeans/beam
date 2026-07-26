#include "beam/objects/button.h"
#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"

namespace beam {
void Button::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }
  Font font;
  if (manager->hasAsset(fontName)) {
    font = manager->getAsset<Font>(fontName);
  } else {
    font = GetFontDefault();
  }
  const float width = size.x + padding.left + padding.right;
  const float height = size.y + padding.top + padding.bottom;
  const Vector2 text_position = {this->position.x + padding.left,
                                 this->position.y + padding.top};

  if (isHovered()) {
    DrawRectangle(position.x, position.y, width, height, bgHover);
    DrawTextEx(font, text.c_str(), text_position, fontSize, fontSpacing, fgHover);
  } else {
    DrawRectangle(position.x, position.y, width, height, bg);
    DrawTextEx(font, text.c_str(), text_position, fontSize, fontSpacing, fg);
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

void Button::onUpdate(
    std::function<void(float, Button *, SharedManager)> callback) {
  updateCallback = callback;
}

void Button::onHover(std::function<void(Button *, SharedManager)> callback) {
  hoverCallback = callback;
}

void Button::onClick(std::function<void(Button *, SharedManager)> callback) {
  clickCallback = callback;
}

void Button::update(float dt, SharedManager manager) {
  GameObject::update(dt, manager);
  if (!active) {
    return;
  }
  if (updateCallback) {
    updateCallback(dt, this, manager);
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

void Button::setFontName(const std::string &fontName) { this->fontName = fontName; }

void Button::setFontSize(float fontSize) { this->fontSize = fontSize; }

void Button::setFontSpacing(float fontSpacing) { this->fontSpacing = fontSpacing; }

void Button::setPosition(Vector2 position) { this->position = position; }

void Button::setSize(Vector2 size) { this->size = size; }

void Button::setPadding(Padding padding) { this->padding = padding; }

void Button::setBgColor(Color color) { bg = color; }

void Button::setFgColor(Color color) { fg = color; }

void Button::setBgHoverColor(Color color) { bgHover = color; }

void Button::setFgHoverColor(Color color) { fgHover = color; }

} // namespace beam