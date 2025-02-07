// Scene.hpp
#pragma once

#include "beam/core/node.h"
#include <memory>

namespace beam {

class Scene : public Node {
private:
  std::vector<std::shared_ptr<Node>> buffer;

public:
  explicit Scene(const std::string &name = "Scene");

  template <typename T> Scene &operator<<(std::shared_ptr<T> child) {
    static_assert(std::is_base_of<Node, T>::value,
                  "Child must be derived from Node");
    buffer.push_back(child);
    return *this;
  }

  void update(float deltaTime) override;
  void draw() override;

  virtual void onEnter();
  virtual void onExit();
};

} // namespace beam
