#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>

namespace beam {
class Button : public GameObject {
protected:
  std::string text;
  float fontSize;
  Vector2 position;
  Vector2 size;
  Padding padding;
  Color bg;
  Color fg;
  Color bgHover;
  Color fgHover;
  std::function<void(Button *, SharedManager)> updateCallback;
  std::function<void(Button *, SharedManager)> hoverCallback;
  std::function<void(Button *, SharedManager)> clickCallback;

public:
  /**
   * @brief Construct a new Button object
   */
  Button(const std::string &text, float fontSize, Vector2 position,
         Vector2 size, Padding padding, Color bg, Color fg, Color bgHover,
         Color fgHover)
      : text(text), fontSize(fontSize), position(position), size(size),
        padding(padding), bg(bg), fg(fg), bgHover(bgHover), fgHover(fgHover),
        updateCallback(nullptr), hoverCallback(nullptr),
        clickCallback(nullptr) {}

  void draw(SharedManager) override;
  void update(float, SharedManager) override;

  void setText(const std::string &text);
  void setFontSize(float fontSize);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);
  void setPadding(Padding padding);
  void setBgColor(Color color);
  void setFgColor(Color color);
  void setBgHoverColor(Color color);
  void setFgHoverColor(Color color);

  const Rectangle getBounds() const;

  bool isHovered() const;
  bool isClicked() const;

  void onUpdate(std::function<void(Button *, SharedManager)> callback);
  void onHover(std::function<void(Button *, SharedManager)> callback);
  void onClick(std::function<void(Button *, SharedManager)> callback);
};
} // namespace beam