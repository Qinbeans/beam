// GameObject.hpp
#pragma once

#include "beam/core/node.h"

namespace beam {

/**
 * @brief A struct to represent padding
 * @param top The padding on the top
 * @param left The padding on the left
 * @param bottom The padding on the bottom
 * @param right The padding on the right
 */
struct Padding {
  int top;
  int left;
  int bottom;
  int right;
};

class GameObject : public Node {
public:
  explicit GameObject(const std::string &name = "GameObject");
};

} // namespace beam
