
// src/core/Event.cpp
#include "beam/core/event.h"

namespace beam {

Event::Event(const std::string &name) : Node(name) {}

void Event::update(float deltaTime) {
  for (const auto &child : buffer) {
    child->update(deltaTime);
  }
}

void Event::draw() {
  for (const auto &child : buffer) {
    child->draw();
  }
}

} // namespace beam