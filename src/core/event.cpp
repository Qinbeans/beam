
// src/core/Event.cpp
#include "beam/core/event.h"
#include "beam/core/manager.h"
#include "beam/core/node.h"

namespace beam {

Event::Event(const std::string &name) : Node(name) {}

void Event::init(SharedManager manager) {
  Node::init(manager);
  for (const auto &child : buffer) {
    child->init(manager);
  }
}

void Event::update(float deltaTime, SharedManager manager) {
  Node::update(deltaTime, manager);
  for (const auto &child : buffer) {
    child->update(deltaTime, manager);
  }
}

void Event::draw(SharedManager manager) {
  Node::draw(manager);
  for (const auto &child : buffer) {
    child->draw(manager);
  }
}

} // namespace beam