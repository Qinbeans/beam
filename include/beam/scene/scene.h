// Scene.hpp
#pragma once

#include "beam/core/manager.h"
#include "beam/core/node.h"
#include <memory>
#include <vector>

namespace beam {

class Scene : public Node {
protected:
  std::vector<std::shared_ptr<Node>> buffer;

public:
  explicit Scene(const std::string &name = "Scene");

  template <typename T> Scene &operator<<(std::shared_ptr<T> child) {
    static_assert(std::is_base_of<Node, T>::value,
                  "Child must be derived from Node");
    if (child) {
      // We need to ensure that 'this' is already owned by a shared_ptr
      try {
        auto sharedThis = shared_from_this();
        child->setParent(sharedThis); // Set weak_ptr to parent
        buffer.push_back(child);
      } catch (const std::bad_weak_ptr &) {
        // If we get here, the Scene wasn't properly created with make_shared
        throw std::runtime_error(
            "Scene must be created with make_shared before adding children");
      }
    }
    return *this;
  }

  template <typename T> T *getChildAs(int index) {
    return dynamic_cast<T *>(buffer[index].get());
  }

  void init(SharedManager) override;
  void update(float, SharedManager) override;
  void draw(SharedManager) override;

  virtual void onEnter(SharedManager);
  virtual void onExit(SharedManager);
};

} // namespace beam
