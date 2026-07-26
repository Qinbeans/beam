#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <string>

namespace beam {
class CheckBox : public GameObject {
protected:
  std::string text;
  Vector2 position;
  Vector2 size;
  bool checked;
  Color bg;
  Color fg;
  std::string fontName;
  float fontSize;
  float fontSpacing;

  std::function<void(CheckBox *, SharedManager)> changeCallback;

public:
  CheckBox(const std::string &text, Vector2 position, Vector2 size,
            bool checked = false, Color bg = LIGHTGRAY, Color fg = DARKGRAY,
            const std::string &fontName = "default", float fontSize = 20.0f,
            float fontSpacing = 1.0f)
      : text(text), position(position), size(size), checked(checked), bg(bg),
        fg(fg), fontName(fontName), fontSize(fontSize),
        fontSpacing(fontSpacing) {}

  void draw(SharedManager) override;

  void setText(const std::string &text);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);
  void setChecked(bool checked);
  void setBgColor(Color color);
  void setFgColor(Color color);
  void setFontName(const std::string &fontName);
  void setFontSize(float fontSize);
  void setFontSpacing(float fontSpacing);

  const std::string &getText() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;
  bool isChecked() const;
  Color getBgColor() const;
  Color getFgColor() const;

  const Rectangle getBounds() const;

  void onChange(std::function<void(CheckBox *, SharedManager)> callback);
};
} // namespace beam
