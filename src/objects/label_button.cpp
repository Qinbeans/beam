#include "beam/objects/label_button.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle LabelButton::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void LabelButton::draw(SharedManager manager) {
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
  int prevTextNormal = GuiGetStyle(LABEL, TEXT_COLOR_NORMAL);

  GuiSetFont(font);
  GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(fontSize));
  GuiSetStyle(DEFAULT, TEXT_SPACING, static_cast<int>(fontSpacing));
  GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(fg));

  if (GuiLabelButton(getBounds(), text.c_str()) && clickCallback) {
    clickCallback(this, manager);
  }

  GuiSetFont(prevFont);
  GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
  GuiSetStyle(DEFAULT, TEXT_SPACING, prevTextSpacing);
  GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, prevTextNormal);
}

void LabelButton::setText(const std::string &text) { this->text = text; }

void LabelButton::setPosition(Vector2 position) { this->position = position; }

void LabelButton::setSize(Vector2 size) { this->size = size; }

void LabelButton::setFgColor(Color color) { fg = color; }

void LabelButton::setFontName(const std::string &fontName) { this->fontName = fontName; }

void LabelButton::setFontSize(float fontSize) { this->fontSize = fontSize; }

void LabelButton::setFontSpacing(float fontSpacing) { this->fontSpacing = fontSpacing; }

const std::string &LabelButton::getText() const { return text; }

Vector2 LabelButton::getPosition() const { return position; }

Vector2 LabelButton::getSize() const { return size; }

Color LabelButton::getFgColor() const { return fg; }

void LabelButton::onClick(std::function<void(LabelButton *, SharedManager)> callback) {
  clickCallback = callback;
}

} // namespace beam
