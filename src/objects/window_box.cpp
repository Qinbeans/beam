#include "beam/objects/window_box.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle WindowBox::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void WindowBox::init(SharedManager manager) {
  GameObject::init(manager);
  for (const auto &child : buffer) {
    child->init(manager);
  }
}

void WindowBox::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  int prevBaseNormal = GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL);
  GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(STATUSBAR, TEXT_COLOR_NORMAL, ColorToInt(fg));

  bool closed = GuiWindowBox(getBounds(), title.c_str());

  GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(STATUSBAR, TEXT_COLOR_NORMAL, prevTextNormal);

  if (closed && closeCallback) {
    closeCallback(this, manager);
  }

  for (const auto &child : buffer) {
    child->draw(manager);
  }
}

void WindowBox::update(float dt, SharedManager manager) {
  GameObject::update(dt, manager);
  for (const auto &child : buffer) {
    child->update(dt, manager);
  }
}

void WindowBox::setTitle(const std::string &title) { this->title = title; }

void WindowBox::setPosition(Vector2 position) { this->position = position; }

void WindowBox::setSize(Vector2 size) { this->size = size; }

void WindowBox::setBgColor(Color color) { bg = color; }

void WindowBox::setFgColor(Color color) { fg = color; }

const std::string &WindowBox::getTitle() const { return title; }

Vector2 WindowBox::getPosition() const { return position; }

Vector2 WindowBox::getSize() const { return size; }

Color WindowBox::getBgColor() const { return bg; }

Color WindowBox::getFgColor() const { return fg; }

void WindowBox::onClose(std::function<void(WindowBox *, SharedManager)> callback) {
  closeCallback = callback;
}

} // namespace beam
