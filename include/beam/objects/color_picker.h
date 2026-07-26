#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>

namespace beam {
class ColorPicker : public GameObject {
protected:
  Color color;
  Vector2 position;
  Vector2 size;

  std::function<void(ColorPicker *, SharedManager)> changeCallback;

public:
  ColorPicker(Vector2 position, Vector2 size, Color color = RED)
      : color(color), position(position), size(size) {}

  void draw(SharedManager) override;

  void setColor(Color color);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);

  Color getColor() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;

  const Rectangle getBounds() const;

  void onChange(std::function<void(ColorPicker *, SharedManager)> callback);
};
} // namespace beam
