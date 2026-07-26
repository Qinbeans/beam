#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <string>

namespace beam {
class ValueBoxFloat : public GameObject {
protected:
  std::string text;
  Vector2 position;
  Vector2 size;
  float value;
  bool editMode;
  Color bg;
  Color fg;
  std::string fontName;
  float fontSize;
  float fontSpacing;
  char textValue[32];

  std::function<void(ValueBoxFloat *, SharedManager)> changeCallback;

public:
  ValueBoxFloat(const std::string &text, Vector2 position, Vector2 size,
                float value = 0.0f, Color bg = LIGHTGRAY, Color fg = DARKGRAY,
                const std::string &fontName = "default",
                float fontSize = 20.0f, float fontSpacing = 1.0f);

  void draw(SharedManager) override;

  void setText(const std::string &text);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);
  void setValue(float value);
  void setEditMode(bool editMode);
  void setBgColor(Color color);
  void setFgColor(Color color);
  void setFontName(const std::string &fontName);
  void setFontSize(float fontSize);
  void setFontSpacing(float fontSpacing);

  const std::string &getText() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;
  float getValue() const;
  bool isEditMode() const;
  Color getBgColor() const;
  Color getFgColor() const;

  const Rectangle getBounds() const;

  void onChange(std::function<void(ValueBoxFloat *, SharedManager)> callback);
};
} // namespace beam
