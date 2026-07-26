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

  int prevBaseNormal = GuiGetStyle(TOGGLE, BASE_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(TOGGLE, TEXT_COLOR_NORMAL);
  int prevBaseFocused = GuiGetStyle(TOGGLE, BASE_COLOR_FOCUSED);
  int prevTextFocused = GuiGetStyle(TOGGLE, TEXT_COLOR_FOCUSED);
  int prevBasePressed = GuiGetStyle(TOGGLE, BASE_COLOR_PRESSED);
  int prevTextPressed = GuiGetStyle(TOGGLE, TEXT_COLOR_PRESSED);
  GuiSetStyle(TOGGLE, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(TOGGLE, TEXT_COLOR_NORMAL, ColorToInt(fg));
  GuiSetStyle(TOGGLE, BASE_COLOR_FOCUSED, ColorToInt(bg));
  GuiSetStyle(TOGGLE, TEXT_COLOR_FOCUSED, ColorToInt(fg));
  GuiSetStyle(TOGGLE, BASE_COLOR_PRESSED, ColorToInt(bgActive));
  GuiSetStyle(TOGGLE, TEXT_COLOR_PRESSED, ColorToInt(fgActive));

  int prevActiveIndex = activeIndex;
  std::vector<char *> ptrs = toCStrArray(items);
  GuiTabBarEx(getBounds(), ptrs.data(), static_cast<int>(ptrs.size()),
              &hscroll, &activeIndex, &focusIndex);

  GuiSetStyle(TOGGLE, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(TOGGLE, TEXT_COLOR_NORMAL, prevTextNormal);
  GuiSetStyle(TOGGLE, BASE_COLOR_FOCUSED, prevBaseFocused);
  GuiSetStyle(TOGGLE, TEXT_COLOR_FOCUSED, prevTextFocused);
  GuiSetStyle(TOGGLE, BASE_COLOR_PRESSED, prevBasePressed);
  GuiSetStyle(TOGGLE, TEXT_COLOR_PRESSED, prevTextPressed);

  if (activeIndex != prevActiveIndex && changeCallback) {
    changeCallback(this, manager);
  }
}

void TabBar::setItems(const std::vector<std::string> &items) { this->items = items; }

void TabBar::setPosition(Vector2 position) { this->position = position; }

void TabBar::setSize(Vector2 size) { this->size = size; }

void TabBar::setHscroll(int hscroll) { this->hscroll = hscroll; }

void TabBar::setActive(int active) { activeIndex = active; }

void TabBar::setBgColor(Color color) { bg = color; }

void TabBar::setFgColor(Color color) { fg = color; }

void TabBar::setBgActiveColor(Color color) { bgActive = color; }

void TabBar::setFgActiveColor(Color color) { fgActive = color; }

const std::vector<std::string> &TabBar::getItems() const { return items; }

Vector2 TabBar::getPosition() const { return position; }

Vector2 TabBar::getSize() const { return size; }

int TabBar::getHscroll() const { return hscroll; }

int TabBar::getActive() const { return activeIndex; }

int TabBar::getFocusIndex() const { return focusIndex; }

Color TabBar::getBgColor() const { return bg; }

Color TabBar::getFgColor() const { return fg; }

Color TabBar::getBgActiveColor() const { return bgActive; }

Color TabBar::getFgActiveColor() const { return fgActive; }

void TabBar::onChange(std::function<void(TabBar *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
