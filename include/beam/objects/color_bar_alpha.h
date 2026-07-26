#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>

namespace beam {
class ColorBarAlpha : public GameObject {
protected:
  float alpha;
  Vector2 position;
  Vector2 size;

  std::function<void(ColorBarAlpha *, SharedManager)> changeCallback;

public:
  ColorBarAlpha(Vector2 position, Vector2 size, float alpha = 1.0f)
      : alpha(alpha), position(position), size(size) {}

  void draw(SharedManager) override;

  void setAlpha(float alpha);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);

  float getAlpha() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;

  const Rectangle getBounds() const;

  void onChange(std::function<void(ColorBarAlpha *, SharedManager)> callback);
};
} // namespace beam
