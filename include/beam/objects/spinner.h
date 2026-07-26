#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <string>

namespace beam {
class Spinner : public GameObject {
protected:
  std::string text;
  Vector2 position;
  Vector2 size;
  int value;
  int minValue;
  int maxValue;
  bool editMode;
  Color bg;
  Color fg;
  std::string fontName;
  float fontSize;
  float fontSpacing;

  std::function<void(Spinner *, SharedManager)> changeCallback;

public:
  Spinner(const std::string &text, Vector2 position, Vector2 size,
          int value = 0, int minValue = 0, int maxValue = 100,
          Color bg = LIGHTGRAY, Color fg = DARKGRAY,
          const std::string &fontName = "default", float fontSize = 20.0f,
          float fontSpacing = 1.0f)
      : text(text), position(position), size(size), value(value),
        minValue(minValue), maxValue(maxValue), editMode(false), bg(bg),
        fg(fg), fontName(fontName), fontSize(fontSize),
        fontSpacing(fontSpacing) {}

  void draw(SharedManager) override;

  void setText(const std::string &text);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);
  void setValue(int value);
  void setRange(int minValue, int maxValue);
  void setEditMode(bool editMode);
  void setBgColor(Color color);
  void setFgColor(Color color);
  void setFontName(const std::string &fontName);
  void setFontSize(float fontSize);
  void setFontSpacing(float fontSpacing);

  const std::string &getText() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;
  int getValue() const;
  int getMinValue() const;
  int getMaxValue() const;
  bool isEditMode() const;
  Color getBgColor() const;
  Color getFgColor() const;

  const Rectangle getBounds() const;

  void onChange(std::function<void(Spinner *, SharedManager)> callback);
};
} // namespace beam
