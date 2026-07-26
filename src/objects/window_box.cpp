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

  if (GuiWindowBox(getBounds(), title.c_str()) && closeCallback) {
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

const std::string &WindowBox::getTitle() const { return title; }

Vector2 WindowBox::getPosition() const { return position; }

Vector2 WindowBox::getSize() const { return size; }

void WindowBox::onClose(std::function<void(WindowBox *, SharedManager)> callback) {
  closeCallback = callback;
}

} // namespace beam
