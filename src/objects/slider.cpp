#include "beam/objects/slider.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle Slider::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void Slider::draw(SharedManager manager) {
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
  GuiSlider(getBounds(), textLeft.empty() ? nullptr : textLeft.c_str(),
            textRight.empty() ? nullptr : textRight.c_str(), &value, minValue,
            maxValue);
  if (value != prevValue && changeCallback) {
    changeCallback(this, manager);
  }

  GuiSetStyle(SLIDER, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(SLIDER, BORDER_COLOR_NORMAL, prevBorderNormal);
  GuiSetStyle(SLIDER, BASE_COLOR_PRESSED, prevBaseFilled);
}

void Slider::setPosition(Vector2 position) { this->position = position; }

void Slider::setSize(Vector2 size) { this->size = size; }

void Slider::setValue(float value) { this->value = value; }

void Slider::setRange(float minValue, float maxValue) {
  this->minValue = minValue;
  this->maxValue = maxValue;
}

void Slider::setBgColor(Color color) { bg = color; }

void Slider::setFgColor(Color color) { fg = color; }

void Slider::setTextLeft(const std::string &text) { textLeft = text; }

void Slider::setTextRight(const std::string &text) { textRight = text; }

Vector2 Slider::getPosition() const { return position; }

Vector2 Slider::getSize() const { return size; }

float Slider::getValue() const { return value; }

float Slider::getMinValue() const { return minValue; }

float Slider::getMaxValue() const { return maxValue; }

Color Slider::getBgColor() const { return bg; }

Color Slider::getFgColor() const { return fg; }

void Slider::onChange(std::function<void(Slider *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
