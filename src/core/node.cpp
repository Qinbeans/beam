// src/core/Node.cpp
#include "beam/core/node.h"

namespace beam {

Node::Node(const std::string &nodeName)
    : position{0, 0}, active(true), name(nodeName) {}

void Node::update(float, SharedManager) {
  if (!active)
    return;
}

void Node::draw(SharedManager) {
  if (!active)
    return;
}

void Node::setPosition(float x, float y) { position = {x, y}; }

void Node::setActive(bool state) { active = state; }

void Node::setParent(std::shared_ptr<Node> parentNode) { parent = parentNode; }

Vector2 Node::getPosition() const { return position; }

bool Node::isActive() const { return active; }

const std::string &Node::getName() const { return name; }

std::weak_ptr<Node> Node::getParent() const { return parent; }

} // namespace beam