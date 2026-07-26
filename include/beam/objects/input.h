#pragma once

#include "beam/core/manager.h"
#include "beam/objects/button.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <string>

namespace beam {
class Input : public Button {
private:
  std::string content;
  std::string placeholder;

  bool cursorVisible;
  bool focused;
  float cursorBlinkTime;

  std::function<void(Input *, SharedManager)> focusCallback;
  std::function<void(Input *, SharedManager)> blurCallback;

public:
  /**
   * @brief Construct a new Input object
   */
  Input(const std::string &placeholder, float fontSize, float fontSpacing,
        Vector2 position, Vector2 size, Padding padding, Color bg, Color fg,
        Color bgHover, Color fgHover, float cursorBlinkTime = 0.5f,
        const std::string &fontName = "default")
      : Button("", fontSize, fontSpacing, position, size, padding, bg, fg,
               bgHover, fgHover, fontName),
        content(""), placeholder(placeholder), cursorVisible(false),
        cursorBlinkTime(cursorBlinkTime) {}

  void draw(SharedManager) override;
  void update(float, SharedManager) override;

  void setContent(const std::string &content);
  void setFocused(bool focused);

  const std::string &getContent() const;
  bool isFocused() const;

  void onFocus(std::function<void(Input *, SharedManager)>);
  void onBlur(std::function<void(Input *, SharedManager)>);
};
} // namespace beam