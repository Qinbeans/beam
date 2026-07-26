#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <string>
#include <vector>

namespace beam {
class TabBar : public GameObject {
protected:
  std::vector<std::string> items;
  Vector2 position;
  Vector2 size;
  int hscroll;
  int activeIndex;
  int focusIndex;

  std::function<void(TabBar *, SharedManager)> changeCallback;

public:
  TabBar(const std::vector<std::string> &items, Vector2 position,
         Vector2 size, int active = 0)
      : items(items), position(position), size(size), hscroll(0),
        activeIndex(active), focusIndex(-1) {}

  void draw(SharedManager) override;

  void setItems(const std::vector<std::string> &items);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);
  void setHscroll(int hscroll);
  void setActive(int active);

  const std::vector<std::string> &getItems() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;
  int getHscroll() const;
  int getActive() const;
  int getFocusIndex() const;

  const Rectangle getBounds() const;

  void onChange(std::function<void(TabBar *, SharedManager)> callback);
};
} // namespace beam
