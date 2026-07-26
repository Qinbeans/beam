#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <string>

namespace beam {
class DummyRec : public GameObject {
protected:
  std::string text;
  Vector2 position;
  Vector2 size;
  Color bg;
  Color fg;

  std::function<void(DummyRec *, SharedManager)> clickCallback;

public:
  DummyRec(const std::string &text, Vector2 position, Vector2 size,
           Color bg = LIGHTGRAY, Color fg = DARKGRAY)
      : text(text), position(position), size(size), bg(bg), fg(fg) {}

  void draw(SharedManager) override;

  void setText(const std::string &text);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);
  void setBgColor(Color color);
  void setFgColor(Color color);

  const std::string &getText() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;
  Color getBgColor() const;
  Color getFgColor() const;

  const Rectangle getBounds() const;

  void onClick(std::function<void(DummyRec *, SharedManager)> callback);
};
} // namespace beam
