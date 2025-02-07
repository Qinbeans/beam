#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"

namespace beam {
class Button : public GameObject {
private:
  std::string text;
  Vector2 position;
  Vector2 size;
  Padding padding;
  Color bg;
  Color fg;
  Color bgHover;
  Color fgHover;
  bool hovered;
  bool clicked;
  std::function<void(Button *, SharedManager)> updateCallback;
  std::function<void(Button *, SharedManager)> hoverCallback;
  std::function<void(Button *, SharedManager)> clickCallback;

public:
  Button(const std::string &text, Vector2 position, Vector2 size,
         Padding padding, Color bg, Color fg, Color bgHover, Color fgHover)
      : text(text), position(position), size(size), padding(padding), bg(bg),
        fg(fg), bgHover(bgHover), fgHover(fgHover), hovered(false),
        clicked(false), updateCallback(nullptr), hoverCallback(nullptr),
        clickCallback(nullptr) {}

  void draw(SharedManager) override;
  void update(float, SharedManager) override;

  void setText(const std::string &text);
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