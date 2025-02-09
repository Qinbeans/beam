#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>

namespace beam {

class Sprite : public GameObject {
private:
  Vector2 position;
  Rectangle bound;
  Color tint;
  Color bg;
  float rotation;
  float scale;

  std::function<void(float, Sprite *, SharedManager)> updateCallback;
  std::function<void(Sprite *, SharedManager)> drawCallback;

public:
  Sprite(SharedManager manager, const std::string &name, Vector2 position,
         Rectangle bound, Color tint = WHITE, Color bg = BLANK);

  ~Sprite();

  void draw(SharedManager) override;
  void update(float, SharedManager) override;

  void setPosition(Vector2);
  void setBound(Rectangle);
  void setTint(Color);
  void setBackgroundColor(Color);
  void setRotation(float);
  void setScale(float);

  Vector2 getPosition() const;
  Rectangle getBound() const;
  Color getTint() const;
  Color getBackgroundColor() const;
  float getRotation() const;
  float getScale() const;

  void onUpdate(std::function<void(float, Sprite *, SharedManager)>);
  void onDraw(std::function<void(Sprite *, SharedManager)>);
};
} // namespace beam