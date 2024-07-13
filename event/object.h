#ifndef OBJECT_H
#define OBJECT_H

#include "node.h"
#include <raymath.h>

// Forward declaration, to avoid circular dependencies
class Event;

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

class Object: public Node {
    friend Event;
    public:
        Object() { };
        string getType() { return "Object"; }
        void setPosition(Vector2 position) { this->position = position; }
};

#endif // OBJECT_H