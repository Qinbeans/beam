#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>

namespace beam {
class ColorBarHue : public GameObject {
protected:
  float hue;
  Vector2 position;
  Vector2 size;

  std::function<void(ColorBarHue *, SharedManager)> changeCallback;

public:
  ColorBarHue(Vector2 position, Vector2 size, float hue = 0.0f)
      : hue(hue), position(position), size(size) {}

  void draw(SharedManager) override;

  void setHue(float hue);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);

  float getHue() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;

  const Rectangle getBounds() const;

  void onChange(std::function<void(ColorBarHue *, SharedManager)> callback);
};
} // namespace beam
