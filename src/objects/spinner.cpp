#include "beam/objects/spinner.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle Spinner::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void Spinner::draw(SharedManager manager) {
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
  if (GuiSpinner(getBounds(), text.empty() ? nullptr : text.c_str(), &value,
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

void Spinner::setText(const std::string &text) { this->text = text; }

void Spinner::setPosition(Vector2 position) { this->position = position; }

void Spinner::setSize(Vector2 size) { this->size = size; }

void Spinner::setValue(int value) { this->value = value; }

void Spinner::setRange(int minValue, int maxValue) {
  this->minValue = minValue;
  this->maxValue = maxValue;
}

void Spinner::setEditMode(bool editMode) { this->editMode = editMode; }

void Spinner::setBgColor(Color color) { bg = color; }

void Spinner::setFgColor(Color color) { fg = color; }

void Spinner::setFontName(const std::string &fontName) { this->fontName = fontName; }

void Spinner::setFontSize(float fontSize) { this->fontSize = fontSize; }

void Spinner::setFontSpacing(float fontSpacing) { this->fontSpacing = fontSpacing; }

const std::string &Spinner::getText() const { return text; }

Vector2 Spinner::getPosition() const { return position; }

Vector2 Spinner::getSize() const { return size; }

int Spinner::getValue() const { return value; }

int Spinner::getMinValue() const { return minValue; }

int Spinner::getMaxValue() const { return maxValue; }

bool Spinner::isEditMode() const { return editMode; }

Color Spinner::getBgColor() const { return bg; }

Color Spinner::getFgColor() const { return fg; }

void Spinner::onChange(std::function<void(Spinner *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
