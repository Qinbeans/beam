#include "beam/core/manager.h"
#include "beam/objects/button.h"
#include "raylib.h"

namespace beam {
inline void Button::draw(SharedManager manager) {
  Font font = manager->getAsset<Font>("buttonFont");
  const float width = size.x + padding.left + padding.right;
  const float height = size.y + padding.top + padding.bottom;
  const Vector2 text_position = {this->position.x + padding.left,
                                 this->position.y + padding.top};

  if (hovered) {
    DrawRectangle(position.x, position.y, width, height, bgHover);
    DrawText(text.c_str(), text_position.x, text_position.y, font.baseSize,
             fgHover);
  } else {
    DrawRectangle(position.x, position.y, width, height, bg);
    DrawText(text.c_str(), text_position.x, text_position.y, font.baseSize, fg);
  }
}

inline const Rectangle Button::getBounds() const {
  return {position.x, position.y,
          static_cast<float>(size.x + padding.left + padding.right),
          static_cast<float>(size.y + padding.top + padding.bottom)};
}

inline bool Button::isHovered() const {
  return CheckCollisionPointRec(GetMousePosition(), getBounds());
}

inline bool Button::isClicked() const {
  return isHovered() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

inline void
Button::onUpdate(std::function<void(Button *, SharedManager)> callback) {
  updateCallback = callback;
}

inline void
Button::onHover(std::function<void(Button *, SharedManager)> callback) {
  hoverCallback = callback;
}

inline void
Button::onClick(std::function<void(Button *, SharedManager)> callback) {
  clickCallback = callback;
}

inline void Button::update(float, SharedManager manager) {
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

inline void Button::setText(const std::string &text) { this->text = text; }

inline void Button::setBgColor(Color color) { bg = color; }

inline void Button::setFgColor(Color color) { fg = color; }

inline void Button::setBgHoverColor(Color color) { bgHover = color; }

inline void Button::setFgHoverColor(Color color) { fgHover = color; }

} // namespace beam