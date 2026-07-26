#include "beam/objects/frame.h"
#include "beam/objects/game_object.h"

namespace beam {
Frame::Frame(const std::string &name, Vector2 origin, Rectangle bound, Color bg)
    : GameObject(name), origin(origin), bound(bound), bg(bg), rotation(0) {}

void Frame::draw(SharedManager managers) {
  GameObject::draw(managers);
  if (!active) {
    return;
  }
  DrawRectanglePro(bound, origin, rotation, bg);
  for (const auto &child : buffer) {
    child->draw(managers);
  }
  if (drawCallback) {
    drawCallback(this, managers);
  }
}

void Frame::init(SharedManager managers) {
  GameObject::init(managers);
  for (const auto &child : buffer) {
    child->init(managers);
  }
}

void Frame::update(float delta, SharedManager managers) {
  GameObject::update(delta, managers);
  for (const auto &child : buffer) {
    child->update(delta, managers);
  }
  if (updateCallback) {
    updateCallback(delta, this, managers);
  }
}

void Frame::setOrigin(Vector2 origin) { this->origin = origin; }

void Frame::setBound(Rectangle bound) { this->bound = bound; }

void Frame::setBackgroundColor(Color bg) { this->bg = bg; }

void Frame::setRotation(float rotation) { this->rotation = rotation; }

Vector2 Frame::getOrigin() const { return origin; }

Rectangle Frame::getBound() const { return bound; }

Color Frame::getBackgroundColor() const { return bg; }

float Frame::getRotation() const { return rotation; }

void Frame::onUpdate(
    std::function<void(float, Frame *, SharedManager)> callback) {
  updateCallback = callback;
}

void Frame::onDraw(std::function<void(Frame *, SharedManager)> callback) {
  drawCallback = callback;
}
} // namespace beam