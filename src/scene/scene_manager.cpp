// SceneManager.cpp
#include "beam/scene/scene_manager.h"

namespace beam {

SceneManager::SceneManager(const std::string &name) : Node(name) {}

void SceneManager::switchToScene(const std::string &name) {
  if (scenes.find(name) == scenes.end())
    return;

  if (currentScene) {
    currentScene->onExit();
    currentScene->setActive(false);
  }

  currentScene = scenes[name];
  currentScene->setActive(true);
  currentScene->onEnter();
}

std::shared_ptr<Scene> SceneManager::getCurrentScene() const {
  return currentScene;
}

std::shared_ptr<Scene> SceneManager::getScene(const std::string &name) const {
  auto it = scenes.find(name);
  return (it != scenes.end()) ? it->second : nullptr;
}

void SceneManager::update(float deltaTime) {
  if (currentScene) {
    currentScene->update(deltaTime);
  }
}

void SceneManager::draw() {
  if (currentScene) {
    currentScene->draw();
  }
}

} // namespace beam