#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <string>

namespace beam {
class Toggle : public GameObject {
protected:
  std::string text;
  Vector2 position;
  Vector2 size;
  bool toggled;
  Color bg;
  Color fg;
  Color bgActive;
  Color fgActive;
  std::string fontName;
  float fontSize;
  float fontSpacing;

  std::function<void(Toggle *, SharedManager)> changeCallback;

public:
  Toggle(const std::string &text, Vector2 position, Vector2 size,
         bool active = false, Color bg = LIGHTGRAY, Color fg = DARKGRAY,
         Color bgActive = DARKGRAY, Color fgActive = WHITE,
         const std::string &fontName = "default", float fontSize = 20.0f,
         float fontSpacing = 1.0f)
      : text(text), position(position), size(size), toggled(active), bg(bg),
        fg(fg), bgActive(bgActive), fgActive(fgActive), fontName(fontName),
        fontSize(fontSize), fontSpacing(fontSpacing) {}

  void draw(SharedManager) override;

  void setText(const std::string &text);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);
  void setActive(bool active);
  void setBgColor(Color color);
  void setFgColor(Color color);
  void setBgActiveColor(Color color);
  void setFgActiveColor(Color color);
  void setFontName(const std::string &fontName);
  void setFontSize(float fontSize);
  void setFontSpacing(float fontSpacing);

  const std::string &getText() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;
  bool isActive() const;
  Color getBgColor() const;
  Color getFgColor() const;
  Color getBgActiveColor() const;
  Color getFgActiveColor() const;

  const Rectangle getBounds() const;

  void onChange(std::function<void(Toggle *, SharedManager)> callback);
};
} // namespace beam
