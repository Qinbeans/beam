// SceneManager.cpp
#include "beam/scene/scene_manager.h"
#include "beam/core/manager.h"

namespace beam {

SceneManager::SceneManager(const std::string &name) : Node(name) {}

void SceneManager::switchToScene(const std::string &name,
                                 SharedManager managers) {
  if (scenes.find(name) == scenes.end())
    return;

  if (currentScene) {
    currentScene->onExit(managers);
    currentScene->setActive(false);
  }

  currentScene = scenes[name];
  currentScene->setActive(true);
  currentScene->onEnter(managers);
}

std::shared_ptr<Scene> SceneManager::getCurrentScene() const {
  return currentScene;
}

std::shared_ptr<Scene> SceneManager::getScene(const std::string &name) const {
  auto it = scenes.find(name);
  return (it != scenes.end()) ? it->second : nullptr;
}

void SceneManager::init(SharedManager managers) {
  for (const auto &child : scenes) {
    child.second->init(managers);
  }
}

void SceneManager::update(float deltaTime, SharedManager managers) {
  if (currentScene) {
    currentScene->update(deltaTime, managers);
  }
}

void SceneManager::draw(SharedManager managers) {
  if (currentScene) {
    currentScene->draw(managers);
  }
}

} // namespace beam