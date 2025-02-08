
// src/core/Event.cpp
#include "beam/core/event.h"
#include "beam/core/manager.h"

namespace beam {

Event::Event(const std::string &name) : Node(name) {}

void Event::init(SharedManager manager) {
  for (const auto &child : buffer) {
    child->init(manager);
  }
}

void Event::update(float deltaTime, SharedManager manager) {
  for (const auto &child : buffer) {
    child->update(deltaTime, manager);
  }
}

void Event::draw(SharedManager manager) {
  for (const auto &child : buffer) {
    child->draw(manager);
  }
}

} // namespace beam