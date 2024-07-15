#include "frame.h"
#include "raylib.h"

Frame::Frame(Vector2 position, Vector2 size, Color fg, Color bg, Padding padding) {
    id = node_id++;
    this->position = position;
    this->size = size;
    this->fg = fg;
    this->bg = bg;
    this->padding = padding;
}

void Frame::init() {
    if (initialized) return;
    initialized = true;
    if (getParent() != nullptr) {
        position.x += getParent()->getPosition().x;
        position.y += getParent()->getPosition().y;
    }
    for (int i = 0; i < children.size(); i++) {
        children[i]->init();
    }
}

void Frame::update() {
    if (!initialized) {
        throw NodeException(getId(),"Frame not initialized: update() called before init()");
    }
    for (int i = 0; i < children.size(); i++) {
        children[i]->update();
    }
}

void Frame::draw() {
    if (!initialized) {
        throw NodeException(getId(),"Frame not initialized: draw() called before init()");
    }
    DrawRectangle(position.x, position.y, size.x, size.y, fg);
    for (int i = 0; i < children.size(); i++) {
        children[i]->draw();
    }
}

void Frame::debug(Debug &debug) {
    if (!initialized) {
        throw NodeException(getId(),"Frame not initialized: debug() called before init()");
    }
    debug << "Frame[" << id << "] parent(" << parent->getId() << ")" << std::endl;
    for (int i = 0; i < children.size(); i++) {
        children[i]->debug(debug);
    }
}