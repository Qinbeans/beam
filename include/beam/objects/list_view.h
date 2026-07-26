#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <string>
#include <vector>

namespace beam {
class ListView : public GameObject {
protected:
  std::vector<std::string> items;
  Vector2 position;
  Vector2 size;
  int scrollIndex;
  int activeIndex;
  int focusIndex;
  Color bg;
  Color fg;

  std::function<void(ListView *, SharedManager)> changeCallback;

public:
  ListView(const std::vector<std::string> &items, Vector2 position,
            Vector2 size, int active = -1, Color bg = LIGHTGRAY,
            Color fg = DARKGRAY)
      : items(items), position(position), size(size), scrollIndex(0),
        activeIndex(active), focusIndex(-1), bg(bg), fg(fg) {}

  void draw(SharedManager) override;

  void setItems(const std::vector<std::string> &items);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);
  void setScrollIndex(int scrollIndex);
  void setActive(int active);
  void setBgColor(Color color);
  void setFgColor(Color color);

  const std::vector<std::string> &getItems() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;
  int getScrollIndex() const;
  int getActive() const;
  Color getBgColor() const;
  Color getFgColor() const;
  int getFocusIndex() const;

  const Rectangle getBounds() const;

  void onChange(std::function<void(ListView *, SharedManager)> callback);
};
} // namespace beam
