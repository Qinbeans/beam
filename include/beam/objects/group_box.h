#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <string>

namespace beam {
class GroupBox : public GameObject {
protected:
  std::string text;
  Vector2 position;
  Vector2 size;
  Color color;

public:
  GroupBox(const std::string &text, Vector2 position, Vector2 size,
           Color color = GRAY)
      : text(text), position(position), size(size), color(color) {}

  void draw(SharedManager) override;

  void setText(const std::string &text);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);
  void setColor(Color color);

  const std::string &getText() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;
  Color getColor() const;

  const Rectangle getBounds() const;
};
} // namespace beam
