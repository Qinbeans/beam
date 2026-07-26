#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <string>
#include <vector>

namespace beam {
class ToggleGroup : public GameObject {
protected:
  std::vector<std::string> items;
  Vector2 position;
  Vector2 size;
  int activeIndex;
  Color bg;
  Color fg;
  Color bgActive;
  Color fgActive;
  std::string fontName;
  float fontSize;
  float fontSpacing;

  std::function<void(ToggleGroup *, SharedManager)> changeCallback;

public:
  ToggleGroup(const std::vector<std::string> &items, Vector2 position,
              Vector2 size, int active = 0, Color bg = LIGHTGRAY,
              Color fg = DARKGRAY, Color bgActive = DARKGRAY,
              Color fgActive = WHITE, const std::string &fontName = "default",
              float fontSize = 20.0f, float fontSpacing = 1.0f)
      : items(items), position(position), size(size), activeIndex(active), bg(bg),
        fg(fg), bgActive(bgActive), fgActive(fgActive), fontName(fontName),
        fontSize(fontSize), fontSpacing(fontSpacing) {}

  void draw(SharedManager) override;

  void setItems(const std::vector<std::string> &items);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);
  void setActive(int active);
  void setBgColor(Color color);
  void setFgColor(Color color);
  void setBgActiveColor(Color color);
  void setFgActiveColor(Color color);
  void setFontName(const std::string &fontName);
  void setFontSize(float fontSize);
  void setFontSpacing(float fontSpacing);

  const std::vector<std::string> &getItems() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;
  int getActive() const;
  Color getBgColor() const;
  Color getFgColor() const;
  Color getBgActiveColor() const;
  Color getFgActiveColor() const;

  const Rectangle getBounds() const;

  void onChange(std::function<void(ToggleGroup *, SharedManager)> callback);
};
} // namespace beam
