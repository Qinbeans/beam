#include "beam/objects/toggle_group.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

namespace {
// raygui's list-based controls take a single string with items separated by
// ';' rather than an array.
std::string joinItems(const std::vector<std::string> &items) {
  std::string joined;
  for (size_t i = 0; i < items.size(); ++i) {
    if (i > 0) {
      joined += ';';
    }
    joined += items[i];
  }
  return joined;
}
} // namespace

const Rectangle ToggleGroup::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void ToggleGroup::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  Font font;
  if (manager->hasAsset(fontName)) {
    font = manager->getAsset<Font>(fontName);
  } else {
    font = GetFontDefault();
  }

  Font prevFont = GuiGetFont();
  int prevTextSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
  int prevTextSpacing = GuiGetStyle(DEFAULT, TEXT_SPACING);
  int prevBaseNormal = GuiGetStyle(TOGGLE, BASE_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(TOGGLE, TEXT_COLOR_NORMAL);
  int prevBaseFocused = GuiGetStyle(TOGGLE, BASE_COLOR_FOCUSED);
  int prevTextFocused = GuiGetStyle(TOGGLE, TEXT_COLOR_FOCUSED);
  int prevBasePressed = GuiGetStyle(TOGGLE, BASE_COLOR_PRESSED);
  int prevTextPressed = GuiGetStyle(TOGGLE, TEXT_COLOR_PRESSED);

  GuiSetFont(font);
  GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(fontSize));
  GuiSetStyle(DEFAULT, TEXT_SPACING, static_cast<int>(fontSpacing));
  GuiSetStyle(TOGGLE, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(TOGGLE, TEXT_COLOR_NORMAL, ColorToInt(fg));
  GuiSetStyle(TOGGLE, BASE_COLOR_FOCUSED, ColorToInt(bg));
  GuiSetStyle(TOGGLE, TEXT_COLOR_FOCUSED, ColorToInt(fg));
  GuiSetStyle(TOGGLE, BASE_COLOR_PRESSED, ColorToInt(bgActive));
  GuiSetStyle(TOGGLE, TEXT_COLOR_PRESSED, ColorToInt(fgActive));

  int prevActive = activeIndex;
  std::string joined = joinItems(items);
  GuiToggleGroup(getBounds(), joined.c_str(), &activeIndex);
  if (activeIndex != prevActive && changeCallback) {
    changeCallback(this, manager);
  }

  GuiSetFont(prevFont);
  GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
  GuiSetStyle(DEFAULT, TEXT_SPACING, prevTextSpacing);
  GuiSetStyle(TOGGLE, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(TOGGLE, TEXT_COLOR_NORMAL, prevTextNormal);
  GuiSetStyle(TOGGLE, BASE_COLOR_FOCUSED, prevBaseFocused);
  GuiSetStyle(TOGGLE, TEXT_COLOR_FOCUSED, prevTextFocused);
  GuiSetStyle(TOGGLE, BASE_COLOR_PRESSED, prevBasePressed);
  GuiSetStyle(TOGGLE, TEXT_COLOR_PRESSED, prevTextPressed);
}

void ToggleGroup::setItems(const std::vector<std::string> &items) { this->items = items; }

void ToggleGroup::setPosition(Vector2 position) { this->position = position; }

void ToggleGroup::setSize(Vector2 size) { this->size = size; }

void ToggleGroup::setActive(int active) { activeIndex = active; }

void ToggleGroup::setBgColor(Color color) { bg = color; }

void ToggleGroup::setFgColor(Color color) { fg = color; }

void ToggleGroup::setBgActiveColor(Color color) { bgActive = color; }

void ToggleGroup::setFgActiveColor(Color color) { fgActive = color; }

void ToggleGroup::setFontName(const std::string &fontName) { this->fontName = fontName; }

void ToggleGroup::setFontSize(float fontSize) { this->fontSize = fontSize; }

void ToggleGroup::setFontSpacing(float fontSpacing) { this->fontSpacing = fontSpacing; }

const std::vector<std::string> &ToggleGroup::getItems() const { return items; }

Vector2 ToggleGroup::getPosition() const { return position; }

Vector2 ToggleGroup::getSize() const { return size; }

int ToggleGroup::getActive() const { return activeIndex; }

Color ToggleGroup::getBgColor() const { return bg; }

Color ToggleGroup::getFgColor() const { return fg; }

Color ToggleGroup::getBgActiveColor() const { return bgActive; }

Color ToggleGroup::getFgActiveColor() const { return fgActive; }

void ToggleGroup::onChange(std::function<void(ToggleGroup *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
