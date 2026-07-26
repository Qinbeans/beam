#include "beam/objects/toggle.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle Toggle::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void Toggle::draw(SharedManager manager) {
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
  int prevBorderWidth = GuiGetStyle(TOGGLE, BORDER_WIDTH);
  int prevBaseNormal = GuiGetStyle(TOGGLE, BASE_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(TOGGLE, TEXT_COLOR_NORMAL);
  int prevBaseFocused = GuiGetStyle(TOGGLE, BASE_COLOR_FOCUSED);
  int prevTextFocused = GuiGetStyle(TOGGLE, TEXT_COLOR_FOCUSED);
  int prevBasePressed = GuiGetStyle(TOGGLE, BASE_COLOR_PRESSED);
  int prevTextPressed = GuiGetStyle(TOGGLE, TEXT_COLOR_PRESSED);

  GuiSetFont(font);
  GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(fontSize));
  GuiSetStyle(DEFAULT, TEXT_SPACING, static_cast<int>(fontSpacing));
  GuiSetStyle(TOGGLE, BORDER_WIDTH, 0);
  GuiSetStyle(TOGGLE, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(TOGGLE, TEXT_COLOR_NORMAL, ColorToInt(fg));
  GuiSetStyle(TOGGLE, BASE_COLOR_FOCUSED, ColorToInt(bg));
  GuiSetStyle(TOGGLE, TEXT_COLOR_FOCUSED, ColorToInt(fg));
  GuiSetStyle(TOGGLE, BASE_COLOR_PRESSED, ColorToInt(bgActive));
  GuiSetStyle(TOGGLE, TEXT_COLOR_PRESSED, ColorToInt(fgActive));

  bool wasToggled = toggled;
  GuiToggle(getBounds(), text.c_str(), &toggled);
  if (toggled != wasToggled && changeCallback) {
    changeCallback(this, manager);
  }

  GuiSetFont(prevFont);
  GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
  GuiSetStyle(DEFAULT, TEXT_SPACING, prevTextSpacing);
  GuiSetStyle(TOGGLE, BORDER_WIDTH, prevBorderWidth);
  GuiSetStyle(TOGGLE, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(TOGGLE, TEXT_COLOR_NORMAL, prevTextNormal);
  GuiSetStyle(TOGGLE, BASE_COLOR_FOCUSED, prevBaseFocused);
  GuiSetStyle(TOGGLE, TEXT_COLOR_FOCUSED, prevTextFocused);
  GuiSetStyle(TOGGLE, BASE_COLOR_PRESSED, prevBasePressed);
  GuiSetStyle(TOGGLE, TEXT_COLOR_PRESSED, prevTextPressed);
}

void Toggle::setText(const std::string &text) { this->text = text; }

void Toggle::setPosition(Vector2 position) { this->position = position; }

void Toggle::setSize(Vector2 size) { this->size = size; }

void Toggle::setActive(bool active) { toggled = active; }

void Toggle::setBgColor(Color color) { bg = color; }

void Toggle::setFgColor(Color color) { fg = color; }

void Toggle::setBgActiveColor(Color color) { bgActive = color; }

void Toggle::setFgActiveColor(Color color) { fgActive = color; }

void Toggle::setFontName(const std::string &fontName) { this->fontName = fontName; }

void Toggle::setFontSize(float fontSize) { this->fontSize = fontSize; }

void Toggle::setFontSpacing(float fontSpacing) { this->fontSpacing = fontSpacing; }

const std::string &Toggle::getText() const { return text; }

Vector2 Toggle::getPosition() const { return position; }

Vector2 Toggle::getSize() const { return size; }

bool Toggle::isActive() const { return toggled; }

Color Toggle::getBgColor() const { return bg; }

Color Toggle::getFgColor() const { return fg; }

Color Toggle::getBgActiveColor() const { return bgActive; }

Color Toggle::getFgActiveColor() const { return fgActive; }

void Toggle::onChange(std::function<void(Toggle *, SharedManager)> callback) {
  changeCallback = callback;
}

} // namespace beam
