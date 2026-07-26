#include "beam/objects/slider_bar.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle SliderBar::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void SliderBar::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  int prevBaseNormal = GuiGetStyle(SLIDER, BASE_COLOR_NORMAL);
  int prevBorderNormal = GuiGetStyle(SLIDER, BORDER_COLOR_NORMAL);
  int prevBaseFilled = GuiGetStyle(SLIDER, BASE_COLOR_PRESSED);

  GuiSetStyle(SLIDER, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(SLIDER, BORDER_COLOR_NORMAL, ColorToInt(fg));
  GuiSetStyle(SLIDER, BASE_COLOR_PRESSED, ColorToInt(fg));

  float prevValue = value;
  GuiSliderBar(getBounds(), textLeft.empty() ? nullptr : textLeft.c_str(),
               textRight.empty() ? nullptr : textRight.c_str(), &value,
               minValue, maxValue);
  if (value != prevValue && changeCallback) {
    changeCallback(this, manager);
  }

  GuiSetStyle(SLIDER, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(SLIDER, BORDER_COLOR_NORMAL, prevBorderNormal);
  GuiSetStyle(SLIDER, BASE_COLOR_PRESSED, prevBaseFilled);
}

void SliderBar::setPosition(Vector2 position) { this->position = position; }

void SliderBar::setSize(Vector2 size) { this->size = size; }

void SliderBar::setValue(float value) { this->value = value; }

void SliderBar::setRange(float minValue, float maxValue) {
  this->minValue = minValue;
  this->maxValue = maxValue;
}

void SliderBar::setBgColor(Color color) { bg = color; }

void SliderBar::setFgColor(Color color) { fg = color; }

void SliderBar::setTextLeft(const std::string &text) { textLeft = text; }

void SliderBar::setTextRight(const std::string &text) { textRight = text; }

Vector2 SliderBar::getPosition() const { return position; }

Vector2 SliderBar::getSize() const { return size; }

float SliderBar::getValue() const { return value; }

float SliderBar::getMinValue() const { return minValue; }

float SliderBar::getMaxValue() const { return maxValue; }

Color SliderBar::getBgColor() const { return bg; }

Color SliderBar::getFgColor() const { return fg; }

void SliderBar::onChange(std::function<void(SliderBar *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
