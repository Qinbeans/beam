#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <string>

namespace beam {
class SliderBar : public GameObject {
protected:
  Vector2 position;
  Vector2 size;
  float value;
  float minValue;
  float maxValue;
  std::string textLeft;
  std::string textRight;
  Color bg;
  Color fg;

  std::function<void(SliderBar *, SharedManager)> changeCallback;

public:
  SliderBar(Vector2 position, Vector2 size, float value = 0.0f,
            float minValue = 0.0f, float maxValue = 1.0f,
            Color bg = LIGHTGRAY, Color fg = DARKGRAY,
            const std::string &textLeft = "",
            const std::string &textRight = "")
      : position(position), size(size), value(value), minValue(minValue),
        maxValue(maxValue), textLeft(textLeft), textRight(textRight), bg(bg),
        fg(fg) {}

  void draw(SharedManager) override;

  void setPosition(Vector2 position);
  void setSize(Vector2 size);
  void setValue(float value);
  void setRange(float minValue, float maxValue);
  void setBgColor(Color color);
  void setFgColor(Color color);
  void setTextLeft(const std::string &text);
  void setTextRight(const std::string &text);

  Vector2 getPosition() const;
  Vector2 getSize() const;
  float getValue() const;
  float getMinValue() const;
  float getMaxValue() const;
  Color getBgColor() const;
  Color getFgColor() const;

  const Rectangle getBounds() const;

  void onChange(std::function<void(SliderBar *, SharedManager)> callback);
};
} // namespace beam
