#include "beam/objects/checkbox.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle CheckBox::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void CheckBox::draw(SharedManager manager) {
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
  int prevBorderNormal = GuiGetStyle(CHECKBOX, BORDER_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(CHECKBOX, TEXT_COLOR_NORMAL);
  int prevLabelText = GuiGetStyle(LABEL, TEXT_COLOR_NORMAL);

  GuiSetFont(font);
  GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(fontSize));
  GuiSetStyle(DEFAULT, TEXT_SPACING, static_cast<int>(fontSpacing));
  GuiSetStyle(CHECKBOX, BORDER_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, ColorToInt(fg));
  GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(fg));

  bool wasChecked = checked;
  GuiCheckBox(getBounds(), text.c_str(), &checked);
  if (checked != wasChecked && changeCallback) {
    changeCallback(this, manager);
  }

  GuiSetFont(prevFont);
  GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
  GuiSetStyle(DEFAULT, TEXT_SPACING, prevTextSpacing);
  GuiSetStyle(CHECKBOX, BORDER_COLOR_NORMAL, prevBorderNormal);
  GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, prevTextNormal);
  GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, prevLabelText);
}

void CheckBox::setText(const std::string &text) { this->text = text; }

void CheckBox::setPosition(Vector2 position) { this->position = position; }

void CheckBox::setSize(Vector2 size) { this->size = size; }

void CheckBox::setChecked(bool checked) { this->checked = checked; }

void CheckBox::setBgColor(Color color) { bg = color; }

void CheckBox::setFgColor(Color color) { fg = color; }

void CheckBox::setFontName(const std::string &fontName) { this->fontName = fontName; }

void CheckBox::setFontSize(float fontSize) { this->fontSize = fontSize; }

void CheckBox::setFontSpacing(float fontSpacing) { this->fontSpacing = fontSpacing; }

const std::string &CheckBox::getText() const { return text; }

Vector2 CheckBox::getPosition() const { return position; }

Vector2 CheckBox::getSize() const { return size; }

bool CheckBox::isChecked() const { return checked; }

Color CheckBox::getBgColor() const { return bg; }

Color CheckBox::getFgColor() const { return fg; }

void CheckBox::onChange(std::function<void(CheckBox *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
