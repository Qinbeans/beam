// src/core/Node.cpp
#include "beam/core/node.h"
#include <algorithm>

namespace beam {

Node::Node(const std::string &nodeName)
    : position{0, 0}, active(true), name(nodeName), time(0.0) {}

void Node::update(float deltaTime) {
  if (!active)
    return;
  time += deltaTime;
}

void Node::draw() {
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