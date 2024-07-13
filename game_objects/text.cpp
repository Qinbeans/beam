#include "text.h"
#include "raylib.h"

Text::Text(string text, float fontSize, Vector2 position, Color fg, Color bg, Padding padding, Font font) {
    id = node_id++;
    this->font = font;
    this->text = text;
    this->fontSize = fontSize;
    this->fg = fg;
    this->bg = bg;
    this->position = position;
    this->padding = padding;
}

void Text::init() {
    if (initialized) return;
    initialized = true;
    if (getParent() != nullptr) {
        position.x += getParent()->getPosition().x;
        position.y += getParent()->getPosition().y;
    }
    size = MeasureTextEx(font, text.c_str(), fontSize, fontSize/3);
    if (size.x == 0 || size.y == 0) {
        // use default
        size = {static_cast<float>(MeasureText(text.c_str(), fontSize)), fontSize};
    }
}

void Text::update() {
    if (!initialized) {
        throw NodeException(getId(),"Text not initialized: update() called before init()");
    }
}

void Text::draw() {
    if (!initialized) {
        throw NodeException(getId(),"Text not initialized: draw() called before init()");
    }
    const int width = size.x + padding.left + padding.right;
    const int height = size.y + padding.top + padding.bottom;
    const Vector2 text_position = {this->position.x + padding.left, this->position.y + padding.top};
    DrawRectangle(position.x, position.y, width, height, bg);
    DrawTextEx(font, text.c_str(), text_position, fontSize, fontSize/3, fg);
}