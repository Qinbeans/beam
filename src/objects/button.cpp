#include "beam/objects/button.h"
#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raygui.h"
#include "raylib.h"

namespace beam {
void Button::draw(SharedManager manager) {
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
  int prevBorderWidth = GuiGetStyle(BUTTON, BORDER_WIDTH);
  int prevBaseNormal = GuiGetStyle(BUTTON, BASE_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(BUTTON, TEXT_COLOR_NORMAL);
  int prevBaseFocused = GuiGetStyle(BUTTON, BASE_COLOR_FOCUSED);
  int prevTextFocused = GuiGetStyle(BUTTON, TEXT_COLOR_FOCUSED);
  int prevBasePressed = GuiGetStyle(BUTTON, BASE_COLOR_PRESSED);
  int prevTextPressed = GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED);

  GuiSetFont(font);
  GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(fontSize));
  GuiSetStyle(DEFAULT, TEXT_SPACING, static_cast<int>(fontSpacing));
  GuiSetStyle(BUTTON, BORDER_WIDTH, 0);
  GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(fg));
  GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(bgHover));
  GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(fgHover));
  GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(bgHover));
  GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(fgHover));

  GuiButton(getBounds(), text.c_str());

  GuiSetFont(prevFont);
  GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
  GuiSetStyle(DEFAULT, TEXT_SPACING, prevTextSpacing);
  GuiSetStyle(BUTTON, BORDER_WIDTH, prevBorderWidth);
  GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, prevTextNormal);
  GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, prevBaseFocused);
  GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, prevTextFocused);
  GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, prevBasePressed);
  GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, prevTextPressed);
}

const Rectangle Button::getBounds() const {
  return {position.x, position.y,
          static_cast<float>(size.x + padding.left + padding.right),
          static_cast<float>(size.y + padding.top + padding.bottom)};
}

Vector2 Button::getSize() const {
  const Rectangle bounds = getBounds();
  return {bounds.width, bounds.height};
}

Padding Button::getPadding() const { return padding; }

bool Button::isHovered() const {
  return CheckCollisionPointRec(GetMousePosition(), getBounds());
}

bool Button::isClicked() const {
  return isHovered() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void Button::onUpdate(
    std::function<void(float, Button *, SharedManager)> callback) {
  updateCallback = callback;
}

void Button::onHover(std::function<void(Button *, SharedManager)> callback) {
  hoverCallback = callback;
}

void Button::onClick(std::function<void(Button *, SharedManager)> callback) {
  clickCallback = callback;
}

void Button::update(float dt, SharedManager manager) {
  GameObject::update(dt, manager);
  if (!active) {
    return;
  }
  if (updateCallback) {
    updateCallback(dt, this, manager);
  }

  if (isHovered()) {
    if (hoverCallback) {
      hoverCallback(this, manager);
    }
  }

  if (isClicked()) {
    if (clickCallback) {
      clickCallback(this, manager);
    }
  }
}

void Button::setText(const std::string &text) { this->text = text; }

void Button::setFontName(const std::string &fontName) { this->fontName = fontName; }

void Button::setFontSize(float fontSize) { this->fontSize = fontSize; }

void Button::setFontSpacing(float fontSpacing) { this->fontSpacing = fontSpacing; }

void Button::setPosition(Vector2 position) { this->position = position; }

void Button::setSize(Vector2 size) { this->size = size; }

void Button::setPadding(Padding padding) { this->padding = padding; }

void Button::setBgColor(Color color) { bg = color; }

void Button::setFgColor(Color color) { fg = color; }

void Button::setBgHoverColor(Color color) { bgHover = color; }

void Button::setFgHoverColor(Color color) { fgHover = color; }

} // namespace beam