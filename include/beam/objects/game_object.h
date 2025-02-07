// GameObject.hpp
#pragma once

#include "beam/core/node.h"

namespace beam {

class GameObject : public Node {
public:
  explicit GameObject(const std::string &name = "GameObject");
};

} // namespace beam
