// Scene.cpp
#include "beam/scene/scene.h"

namespace beam {

Scene::Scene(const std::string &name) : Node(name) {}

void Scene::onEnter() {}
void Scene::onExit() {}

void Scene::update(float deltaTime) {
  for (const auto &child : buffer) {
    child->update(deltaTime);
  }
}

void Scene::draw() {
  for (const auto &child : buffer) {
    child->draw();
  }
}
} // namespace beam
