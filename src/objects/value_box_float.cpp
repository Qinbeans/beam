#include "beam/objects/value_box_float.h"
#include "beam/objects/game_object.h"
#include "raygui.h"
#include <cstdio>

namespace beam {

ValueBoxFloat::ValueBoxFloat(const std::string &text, Vector2 position,
                              Vector2 size, float value, Color bg, Color fg,
                              const std::string &fontName, float fontSize,
                              float fontSpacing)
    : text(text), position(position), size(size), value(value),
      editMode(false), bg(bg), fg(fg), fontName(fontName),
      fontSize(fontSize), fontSpacing(fontSpacing) {
  std::snprintf(textValue, sizeof(textValue), "%g", value);
}

const Rectangle ValueBoxFloat::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void ValueBoxFloat::draw(SharedManager manager) {
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

  // Keep the display buffer in sync with `value` whenever the user isn't
  // actively editing it (e.g. after an external setValue() call); while
  // editing, raygui owns and mutates this buffer directly.
  if (!editMode) {
    std::snprintf(textValue, sizeof(textValue), "%g", value);
  }

  float prevValue = value;
  if (GuiValueBoxFloat(getBounds(), text.empty() ? nullptr : text.c_str(),
                       textValue, &value, editMode)) {
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

void ValueBoxFloat::setText(const std::string &text) { this->text = text; }

void ValueBoxFloat::setPosition(Vector2 position) { this->position = position; }

void ValueBoxFloat::setSize(Vector2 size) { this->size = size; }

void ValueBoxFloat::setValue(float value) {
  this->value = value;
  std::snprintf(textValue, sizeof(textValue), "%g", value);
}

void ValueBoxFloat::setEditMode(bool editMode) { this->editMode = editMode; }

void ValueBoxFloat::setBgColor(Color color) { bg = color; }

void ValueBoxFloat::setFgColor(Color color) { fg = color; }

void ValueBoxFloat::setFontName(const std::string &fontName) { this->fontName = fontName; }

void ValueBoxFloat::setFontSize(float fontSize) { this->fontSize = fontSize; }

void ValueBoxFloat::setFontSpacing(float fontSpacing) { this->fontSpacing = fontSpacing; }

const std::string &ValueBoxFloat::getText() const { return text; }

Vector2 ValueBoxFloat::getPosition() const { return position; }

Vector2 ValueBoxFloat::getSize() const { return size; }

float ValueBoxFloat::getValue() const { return value; }

bool ValueBoxFloat::isEditMode() const { return editMode; }

Color ValueBoxFloat::getBgColor() const { return bg; }

Color ValueBoxFloat::getFgColor() const { return fg; }

void ValueBoxFloat::onChange(std::function<void(ValueBoxFloat *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
