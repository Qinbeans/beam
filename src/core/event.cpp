
// src/core/Event.cpp
#include "beam/core/event.h"
#include "beam/core/asset.h"
#include "beam/core/manager.h"
#include <memory>
#include <unordered_map>

namespace beam {

Event::Event(const std::string &name) : Node(name) {}

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