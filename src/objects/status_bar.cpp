#include "beam/objects/status_bar.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle StatusBar::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void StatusBar::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  int prevBaseNormal = GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL);
  GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(STATUSBAR, TEXT_COLOR_NORMAL, ColorToInt(fg));

  if (GuiStatusBar(getBounds(), text.c_str()) && clickCallback) {
    clickCallback(this, manager);
  }

  GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(STATUSBAR, TEXT_COLOR_NORMAL, prevTextNormal);
}

void StatusBar::setText(const std::string &text) { this->text = text; }

void StatusBar::setPosition(Vector2 position) { this->position = position; }

void StatusBar::setSize(Vector2 size) { this->size = size; }

void StatusBar::setBgColor(Color color) { bg = color; }

void StatusBar::setFgColor(Color color) { fg = color; }

const std::string &StatusBar::getText() const { return text; }

Vector2 StatusBar::getPosition() const { return position; }

Vector2 StatusBar::getSize() const { return size; }

Color StatusBar::getBgColor() const { return bg; }

Color StatusBar::getFgColor() const { return fg; }

void StatusBar::onClick(std::function<void(StatusBar *, SharedManager)> callback) {
  clickCallback = callback;
}

} // namespace beam
