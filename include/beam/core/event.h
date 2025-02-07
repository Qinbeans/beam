// include/beam/core/Event.hpp
#pragma once

#include "node.h"

namespace beam {

/**
 * @brief Event node that manages a buffer of child nodes
 *
 * Event acts as an orchestrator, managing and updating its buffer of child
 * nodes. It provides stream-like syntax for adding nodes to its buffer.
 */
class Event : public Node {
private:
  std::vector<std::shared_ptr<Node>> buffer;

public:
  explicit Event(const std::string &name = "Event");

  /**
   * @brief Stream operator for adding nodes to the buffer
   *
   * Provides a convenient syntax for adding nodes to the event's buffer:
   * event << node1 << node2;
   */
  template <typename T> Event &operator<<(std::shared_ptr<T> child) {
    static_assert(std::is_base_of<Node, T>::value,
                  "Child must be a subclass of Node");
    buffer.push_back(child);
    child->setParent(shared_from_this());
    return *this;
  }

  void update(float deltaTime) override;
  void draw() override;
};

} // namespace beam
