// Scene.cpp
#include "beam/scene/scene.h"
#include <memory>

namespace beam {

Scene::Scene(const std::string &name) : Node(name) {}

void Scene::onEnter(SharedManager) {}
void Scene::onExit(SharedManager) {}

void Scene::update(float deltaTime, SharedManager managers) {
  for (const auto &child : buffer) {
    child->update(deltaTime, managers);
  }
}

void Scene::draw(SharedManager managers) {
  for (const auto &child : buffer) {
    child->draw(managers);
  }
}
} // namespace beam
