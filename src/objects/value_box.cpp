#include "beam/objects/value_box.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle ValueBox::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void ValueBox::draw(SharedManager manager) {
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
  int prevBaseNormal = GuiGetStyle(VALUEBOX, BASE_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(VALUEBOX, TEXT_COLOR_NORMAL);

  GuiSetFont(font);
  GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(fontSize));
  GuiSetStyle(DEFAULT, TEXT_SPACING, static_cast<int>(fontSpacing));
  GuiSetStyle(VALUEBOX, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(VALUEBOX, TEXT_COLOR_NORMAL, ColorToInt(fg));

  int prevValue = value;
  if (GuiValueBox(getBounds(), text.empty() ? nullptr : text.c_str(), &value,
                  minValue, maxValue, editMode)) {
    editMode = !editMode;
  }
  if (value != prevValue && changeCallback) {
    changeCallback(this, manager);
  }

  GuiSetFont(prevFont);
  GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
  GuiSetStyle(DEFAULT, TEXT_SPACING, prevTextSpacing);
  GuiSetStyle(VALUEBOX, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(VALUEBOX, TEXT_COLOR_NORMAL, prevTextNormal);
}

void ValueBox::setText(const std::string &text) { this->text = text; }

void ValueBox::setPosition(Vector2 position) { this->position = position; }

void ValueBox::setSize(Vector2 size) { this->size = size; }

void ValueBox::setValue(int value) { this->value = value; }

void ValueBox::setRange(int minValue, int maxValue) {
  this->minValue = minValue;
  this->maxValue = maxValue;
}

void ValueBox::setEditMode(bool editMode) { this->editMode = editMode; }

void ValueBox::setBgColor(Color color) { bg = color; }

void ValueBox::setFgColor(Color color) { fg = color; }

void ValueBox::setFontName(const std::string &fontName) { this->fontName = fontName; }

void ValueBox::setFontSize(float fontSize) { this->fontSize = fontSize; }

void ValueBox::setFontSpacing(float fontSpacing) { this->fontSpacing = fontSpacing; }

const std::string &ValueBox::getText() const { return text; }

Vector2 ValueBox::getPosition() const { return position; }

Vector2 ValueBox::getSize() const { return size; }

int ValueBox::getValue() const { return value; }

int ValueBox::getMinValue() const { return minValue; }

int ValueBox::getMaxValue() const { return maxValue; }

bool ValueBox::isEditMode() const { return editMode; }

Color ValueBox::getBgColor() const { return bg; }

Color ValueBox::getFgColor() const { return fg; }

void ValueBox::onChange(std::function<void(ValueBox *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
