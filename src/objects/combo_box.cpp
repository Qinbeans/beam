#include "beam/objects/combo_box.h"
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

const Rectangle ComboBox::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void ComboBox::draw(SharedManager manager) {
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
  int prevBaseNormal = GuiGetStyle(COMBOBOX, BASE_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(COMBOBOX, TEXT_COLOR_NORMAL);

  GuiSetFont(font);
  GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(fontSize));
  GuiSetStyle(DEFAULT, TEXT_SPACING, static_cast<int>(fontSpacing));
  GuiSetStyle(COMBOBOX, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(COMBOBOX, TEXT_COLOR_NORMAL, ColorToInt(fg));

  int prevActive = activeIndex;
  std::string joined = joinItems(items);
  GuiComboBox(getBounds(), joined.c_str(), &activeIndex);
  if (activeIndex != prevActive && changeCallback) {
    changeCallback(this, manager);
  }

  GuiSetFont(prevFont);
  GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
  GuiSetStyle(DEFAULT, TEXT_SPACING, prevTextSpacing);
  GuiSetStyle(COMBOBOX, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(COMBOBOX, TEXT_COLOR_NORMAL, prevTextNormal);
}

void ComboBox::setItems(const std::vector<std::string> &items) { this->items = items; }

void ComboBox::setPosition(Vector2 position) { this->position = position; }

void ComboBox::setSize(Vector2 size) { this->size = size; }

void ComboBox::setActive(int active) { activeIndex = active; }

void ComboBox::setBgColor(Color color) { bg = color; }

void ComboBox::setFgColor(Color color) { fg = color; }

void ComboBox::setFontName(const std::string &fontName) { this->fontName = fontName; }

void ComboBox::setFontSize(float fontSize) { this->fontSize = fontSize; }

void ComboBox::setFontSpacing(float fontSpacing) { this->fontSpacing = fontSpacing; }

const std::vector<std::string> &ComboBox::getItems() const { return items; }

Vector2 ComboBox::getPosition() const { return position; }

Vector2 ComboBox::getSize() const { return size; }

int ComboBox::getActive() const { return activeIndex; }

Color ComboBox::getBgColor() const { return bg; }

Color ComboBox::getFgColor() const { return fg; }

void ComboBox::onChange(std::function<void(ComboBox *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
