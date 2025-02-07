// include/beam/core/Node.hpp
#pragma once

#include "beam/core/manager.h"
#include <memory>
#include <raylib.h>
#include <string>

namespace beam {

/**
 * @brief Base node class for all Beam components
 *
 * Node represents a basic element in the Beam hierarchy. It can contain
 * child nodes and be contained within a parent node, forming a tree structure
 * for managing game objects and events.
 */
class Node : public std::enable_shared_from_this<Node> {
protected:
  std::weak_ptr<Node> parent;
  Vector2 position;
  bool active;
  std::string name;

public:
  explicit Node(const std::string &nodeName = "Node");
  virtual ~Node() = default;

  virtual void update(float deltaTime, SharedManager);
  virtual void draw(SharedManager);

  // Setters
  void setPosition(float x, float y);
  void setActive(bool state);
  void setParent(std::shared_ptr<Node> parentNode);

  // Getters
  Vector2 getPosition() const;
  bool isActive() const;
  const std::string &getName() const;
  std::weak_ptr<Node> getParent() const;

  template <typename T> std::shared_ptr<T> parentAs() {
    auto parent = getParent().lock();
    return std::dynamic_pointer_cast<T>(parent);
  }
};

} // namespace beam