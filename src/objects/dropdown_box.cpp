#include "beam/objects/dropdown_box.h"
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

const Rectangle DropdownBox::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void DropdownBox::draw(SharedManager manager) {
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
  int prevBaseNormal = GuiGetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL);

  GuiSetFont(font);
  GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(fontSize));
  GuiSetStyle(DEFAULT, TEXT_SPACING, static_cast<int>(fontSpacing));
  GuiSetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, ColorToInt(fg));

  int prevActiveIndex = activeIndex;
  std::string joined = joinItems(items);
  if (GuiDropdownBox(getBounds(), joined.c_str(), &activeIndex, open)) {
    open = !open;
  }
  if (activeIndex != prevActiveIndex && changeCallback) {
    changeCallback(this, manager);
  }

  GuiSetFont(prevFont);
  GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
  GuiSetStyle(DEFAULT, TEXT_SPACING, prevTextSpacing);
  GuiSetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, prevTextNormal);
}

void DropdownBox::setItems(const std::vector<std::string> &items) { this->items = items; }

void DropdownBox::setPosition(Vector2 position) { this->position = position; }

void DropdownBox::setSize(Vector2 size) { this->size = size; }

void DropdownBox::setActive(int active) { activeIndex = active; }

void DropdownBox::setOpen(bool open) { this->open = open; }

void DropdownBox::setBgColor(Color color) { bg = color; }

void DropdownBox::setFgColor(Color color) { fg = color; }

void DropdownBox::setFontName(const std::string &fontName) { this->fontName = fontName; }

void DropdownBox::setFontSize(float fontSize) { this->fontSize = fontSize; }

void DropdownBox::setFontSpacing(float fontSpacing) { this->fontSpacing = fontSpacing; }

const std::vector<std::string> &DropdownBox::getItems() const { return items; }

Vector2 DropdownBox::getPosition() const { return position; }

Vector2 DropdownBox::getSize() const { return size; }

int DropdownBox::getActive() const { return activeIndex; }

bool DropdownBox::isOpen() const { return open; }

Color DropdownBox::getBgColor() const { return bg; }

Color DropdownBox::getFgColor() const { return fg; }

void DropdownBox::onChange(std::function<void(DropdownBox *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
