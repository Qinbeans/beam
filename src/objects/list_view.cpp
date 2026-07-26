#include "beam/objects/list_view.h"
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

const Rectangle ListView::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void ListView::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  int prevBaseNormal = GuiGetStyle(LISTVIEW, BASE_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(LISTVIEW, TEXT_COLOR_NORMAL);

  GuiSetStyle(LISTVIEW, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(LISTVIEW, TEXT_COLOR_NORMAL, ColorToInt(fg));

  int prevActiveIndex = activeIndex;
  std::vector<char *> ptrs = toCStrArray(items);
  GuiListViewEx(getBounds(), ptrs.data(), static_cast<int>(ptrs.size()),
                &scrollIndex, &activeIndex, &focusIndex);
  if (activeIndex != prevActiveIndex && changeCallback) {
    changeCallback(this, manager);
  }

  GuiSetStyle(LISTVIEW, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(LISTVIEW, TEXT_COLOR_NORMAL, prevTextNormal);
}

void ListView::setItems(const std::vector<std::string> &items) { this->items = items; }

void ListView::setPosition(Vector2 position) { this->position = position; }

void ListView::setSize(Vector2 size) { this->size = size; }

void ListView::setScrollIndex(int scrollIndex) { this->scrollIndex = scrollIndex; }

void ListView::setActive(int active) { activeIndex = active; }

void ListView::setBgColor(Color color) { bg = color; }

void ListView::setFgColor(Color color) { fg = color; }

const std::vector<std::string> &ListView::getItems() const { return items; }

Vector2 ListView::getPosition() const { return position; }

Vector2 ListView::getSize() const { return size; }

int ListView::getScrollIndex() const { return scrollIndex; }

int ListView::getActive() const { return activeIndex; }

Color ListView::getBgColor() const { return bg; }

Color ListView::getFgColor() const { return fg; }

int ListView::getFocusIndex() const { return focusIndex; }

void ListView::onChange(std::function<void(ListView *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
