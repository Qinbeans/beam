#ifndef FRAME_H
#define FRAME_H

#include <raylib.h>
#include <string>
#include <vector>
#include "../event/object.h"

using std::string;
using std::vector;

class Frame: public Object {
    protected:
        int id;
        Color fg;
        Color bg;
        Padding padding; // top, right, bottom, left
        vector<shared_ptr<Node>> children;
        Vector2 size;
    public:
        Frame(Vector2 position, Vector2 size, Color fg, Color bg = BLANK, Padding padding = {0, 0, 0, 0});
        string getType() { return "Frame"; }
        Color getFg() { return fg; }
        Color getBg() { return bg; }
        int getID() { return id; }
        Frame& setFg(Color fg) { this->fg = fg; return *this; }
        Frame& setBg(Color bg) { this->bg = bg; return *this; }
        Frame& setID(int id) { this->id = id; return *this; }
        void draw() override;
        void init() override;
        void update() override;

        friend Frame& operator<<(Frame& frame, Node* node) {
            frame.children.push_back(shared_ptr<Node>(node));
            node->setParent(shared_ptr<Node>(&frame));
            return frame;
        }
};

#endif // FRAME_H