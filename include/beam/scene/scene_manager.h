

// SceneManager.hpp
#pragma once

#include "beam/scene/scene.h"
#include <unordered_map>

namespace beam {

class SceneManager : public Node {
private:
  std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
  std::shared_ptr<Scene> currentScene;

public:
  explicit SceneManager(const std::string &name = "SceneManager");

  template <typename T> SceneManager &operator<<(std::shared_ptr<T> child) {
    static_assert(std::is_base_of<Scene, T>::value,
                  "Child must be derived from Scene");
    scenes[child->getName()] = child;
    child->setParent(shared_from_this());
    return *this;
  }

  void update(float deltaTime) override;
  void draw() override;

  void switchToScene(const std::string &name);
  std::shared_ptr<Scene> getCurrentScene() const;
  std::shared_ptr<Scene> getScene(const std::string &name) const;
};

} // namespace beam
