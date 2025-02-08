#include "beam/objects/input.h"

namespace beam {
void Input::draw(SharedManager manager) {
  if (content.empty()) {
    text = placeholder;
  } else {
    text = content;
  }

  Button::draw(manager);

  if (!content.empty() && focused && cursorVisible) {
    auto textSize = MeasureText(content.c_str(), fontSize);
    DrawRectangle(position.x + padding.left + textSize,
                  position.y + padding.top, 1, textSize, fg);
  }
}

void Input::update(float dt, SharedManager manager) {
  Button::update(dt, manager);

  cursorVisible = focused && ((GetTime() - cursorBlinkTime) > 0.5f);

  if (focused) {
    if (manager->isKeyPressed(KEY_BACKSPACE)) {
      if (!content.empty()) {
        content.pop_back();
      }
    } else {
      int key = manager->getKeyPressed();
      if (key > 0 && key < 256) {
        content.push_back(static_cast<char>(key));
      }
    }
  }

  if (isHovered() && manager->isMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
      !focused) {
    focused = true;
    if (focusCallback) {
      focusCallback(manager);
    }
  }

  if (!isHovered() && manager->isMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
      focused) {
    focused = false;
    if (blurCallback) {
      blurCallback(manager);
    }
  }
}

void Input::setContent(const std::string &content) { this->content = content; }

void Input::setFocused(bool focused) { this->focused = focused; }

const std::string &Input::getContent() const { return content; }

bool Input::isFocused() const { return focused; }

void Input::onFocus(std::function<void(SharedManager)> callback) {
  focusCallback = callback;
}

void Input::onBlur(std::function<void(SharedManager)> callback) {
  blurCallback = callback;
}

} // namespace beam