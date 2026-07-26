#include "beam/objects/dummy_rec.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle DummyRec::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void DummyRec::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  int prevBaseNormal = GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(BUTTON, TEXT_COLOR_NORMAL);
  GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(fg));

  if (GuiDummyRec(getBounds(), text.c_str()) && clickCallback) {
    clickCallback(this, manager);
  }

  GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, prevTextNormal);
}

void DummyRec::setText(const std::string &text) { this->text = text; }

void DummyRec::setPosition(Vector2 position) { this->position = position; }

void DummyRec::setSize(Vector2 size) { this->size = size; }

void DummyRec::setBgColor(Color color) { bg = color; }

void DummyRec::setFgColor(Color color) { fg = color; }

const std::string &DummyRec::getText() const { return text; }

Vector2 DummyRec::getPosition() const { return position; }

Vector2 DummyRec::getSize() const { return size; }

Color DummyRec::getBgColor() const { return bg; }

Color DummyRec::getFgColor() const { return fg; }

void DummyRec::onClick(std::function<void(DummyRec *, SharedManager)> callback) {
  clickCallback = callback;
}

} // namespace beam
