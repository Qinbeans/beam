#include "beam/objects/progress_bar.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle ProgressBar::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void ProgressBar::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  int prevBorderNormal = GuiGetStyle(PROGRESSBAR, BORDER_COLOR_NORMAL);
  int prevBorderFocused = GuiGetStyle(PROGRESSBAR, BORDER_COLOR_FOCUSED);
  int prevBasePressed = GuiGetStyle(PROGRESSBAR, BASE_COLOR_PRESSED);

  GuiSetStyle(PROGRESSBAR, BORDER_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(PROGRESSBAR, BORDER_COLOR_FOCUSED, ColorToInt(fg));
  GuiSetStyle(PROGRESSBAR, BASE_COLOR_PRESSED, ColorToInt(fg));

  GuiProgressBar(getBounds(), textLeft.empty() ? nullptr : textLeft.c_str(),
                 textRight.empty() ? nullptr : textRight.c_str(), &value,
                 minValue, maxValue);

  GuiSetStyle(PROGRESSBAR, BORDER_COLOR_NORMAL, prevBorderNormal);
  GuiSetStyle(PROGRESSBAR, BORDER_COLOR_FOCUSED, prevBorderFocused);
  GuiSetStyle(PROGRESSBAR, BASE_COLOR_PRESSED, prevBasePressed);
}

void ProgressBar::setPosition(Vector2 position) { this->position = position; }

void ProgressBar::setSize(Vector2 size) { this->size = size; }

void ProgressBar::setValue(float value) { this->value = value; }

void ProgressBar::setRange(float minValue, float maxValue) {
  this->minValue = minValue;
  this->maxValue = maxValue;
}

void ProgressBar::setBgColor(Color color) { bg = color; }

void ProgressBar::setFgColor(Color color) { fg = color; }

void ProgressBar::setTextLeft(const std::string &text) { textLeft = text; }

void ProgressBar::setTextRight(const std::string &text) { textRight = text; }

Vector2 ProgressBar::getPosition() const { return position; }

Vector2 ProgressBar::getSize() const { return size; }

float ProgressBar::getValue() const { return value; }

float ProgressBar::getMinValue() const { return minValue; }

float ProgressBar::getMaxValue() const { return maxValue; }

Color ProgressBar::getBgColor() const { return bg; }

Color ProgressBar::getFgColor() const { return fg; }

} // namespace beam
