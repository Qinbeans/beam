#include "beam/objects/tab_bar.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

namespace {
std::vector<char *> toCStrArray(std::vector<std::string> &items) {
  std::vector<char *> ptrs;
  ptrs.reserve(items.size());
  for (auto &s : items) {
    ptrs.push_back(s.data());
  }
  return ptrs;
}
} // namespace

const Rectangle TabBar::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void TabBar::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  int prevActiveIndex = activeIndex;
  std::vector<char *> ptrs = toCStrArray(items);
  GuiTabBarEx(getBounds(), ptrs.data(), static_cast<int>(ptrs.size()),
              &hscroll, &activeIndex, &focusIndex);
  if (activeIndex != prevActiveIndex && changeCallback) {
    changeCallback(this, manager);
  }
}

void TabBar::setItems(const std::vector<std::string> &items) { this->items = items; }

void TabBar::setPosition(Vector2 position) { this->position = position; }

void TabBar::setSize(Vector2 size) { this->size = size; }

void TabBar::setHscroll(int hscroll) { this->hscroll = hscroll; }

void TabBar::setActive(int active) { activeIndex = active; }

const std::vector<std::string> &TabBar::getItems() const { return items; }

Vector2 TabBar::getPosition() const { return position; }

Vector2 TabBar::getSize() const { return size; }

int TabBar::getHscroll() const { return hscroll; }

int TabBar::getActive() const { return activeIndex; }

int TabBar::getFocusIndex() const { return focusIndex; }

void TabBar::onChange(std::function<void(TabBar *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
