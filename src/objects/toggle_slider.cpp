#include "beam/objects/toggle_slider.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

namespace {
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

const Rectangle ToggleSlider::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void ToggleSlider::draw(SharedManager manager) {
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
  int prevBaseNormal = GuiGetStyle(SLIDER, BASE_COLOR_NORMAL);
  int prevBorderNormal = GuiGetStyle(SLIDER, BORDER_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(SLIDER, TEXT_COLOR_NORMAL);

  GuiSetFont(font);
  GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(fontSize));
  GuiSetStyle(DEFAULT, TEXT_SPACING, static_cast<int>(fontSpacing));
  GuiSetStyle(SLIDER, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(SLIDER, BORDER_COLOR_NORMAL, ColorToInt(fg));
  GuiSetStyle(SLIDER, TEXT_COLOR_NORMAL, ColorToInt(fg));

  int prevActive = activeIndex;
  std::string joined = joinItems(items);
  GuiToggleSlider(getBounds(), joined.c_str(), &activeIndex);
  if (activeIndex != prevActive && changeCallback) {
    changeCallback(this, manager);
  }

  GuiSetFont(prevFont);
  GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
  GuiSetStyle(DEFAULT, TEXT_SPACING, prevTextSpacing);
  GuiSetStyle(SLIDER, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(SLIDER, BORDER_COLOR_NORMAL, prevBorderNormal);
  GuiSetStyle(SLIDER, TEXT_COLOR_NORMAL, prevTextNormal);
}

void ToggleSlider::setItems(const std::vector<std::string> &items) { this->items = items; }

void ToggleSlider::setPosition(Vector2 position) { this->position = position; }

void ToggleSlider::setSize(Vector2 size) { this->size = size; }

void ToggleSlider::setActive(int active) { activeIndex = active; }

void ToggleSlider::setBgColor(Color color) { bg = color; }

void ToggleSlider::setFgColor(Color color) { fg = color; }

void ToggleSlider::setFontName(const std::string &fontName) { this->fontName = fontName; }

void ToggleSlider::setFontSize(float fontSize) { this->fontSize = fontSize; }

void ToggleSlider::setFontSpacing(float fontSpacing) { this->fontSpacing = fontSpacing; }

const std::vector<std::string> &ToggleSlider::getItems() const { return items; }

Vector2 ToggleSlider::getPosition() const { return position; }

Vector2 ToggleSlider::getSize() const { return size; }

int ToggleSlider::getActive() const { return activeIndex; }

Color ToggleSlider::getBgColor() const { return bg; }

Color ToggleSlider::getFgColor() const { return fg; }

void ToggleSlider::onChange(std::function<void(ToggleSlider *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
