#include "text_button.h"
#include "raylib.h"

TextButton::TextButton(string text, float fontSize, Vector2 position, Color fg, Color bg, Color fgHover, Color bgHover, Padding padding, Font font) : Text(text, fontSize, position, fg, bg, padding, font) {
    id = node_id++;
    this->onClick = nullptr;
    this->onHover = nullptr;
    this->onUnhover = nullptr;
    this->isHovered = false;
    this->fgHover = fgHover;
    this->bgHover = bgHover;
}

void TextButton::init() {
    if (initialized) return;
    initialized = true;
    if (getParent() != nullptr) {
        position.x += getParent()->getPosition().x;
        position.y += getParent()->getPosition().y;
    }
    if (onClick == nullptr) {
        throw NodeException(id,"onClick is not set");
    }
    if (onHover == nullptr) {
        throw NodeException(id,"onHover is not set");
    }
    if (onUnhover == nullptr) {
        throw NodeException(id,"onUnhover is not set");
    }
    size = MeasureTextEx(font, text.c_str(), fontSize, fontSize/4);
    if (size.x == 0 || size.y == 0) {
        // use default
        size = {static_cast<float>(MeasureText(text.c_str(), fontSize)), fontSize};
    }
}

void TextButton::update() {
    if (!initialized) {
        throw NodeException(getId(),"TextButton not initialized: update() called before init()");
    }
    Rectangle bounds = {position.x, position.y, static_cast<float>(size.x + padding.left + padding.right), static_cast<float>(size.y + padding.top + padding.bottom)};
    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
        if (!isHovered) {
            onHover(this);
            isHovered = true;
        }
    } else {
        if (isHovered) {
            onUnhover(this);
            isHovered = false;
        }
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isHovered) {
        onClick(this);
    }
}

void TextButton::draw() {
    if (!initialized) {
        throw NodeException(getId(),"TextButton not initialized: draw() called before init()");
    }
    const float width = size.x + padding.left + padding.right;
    const float height = size.y + padding.top + padding.bottom;
    const Vector2 text_position = {this->position.x + padding.left, this->position.y + padding.top};
    if (isHovered) {
        DrawRectangle(position.x, position.y, width, height, bgHover);
        DrawTextEx(font, text.c_str(), text_position, fontSize, fontSize/4, fgHover);
        return;
    }
    DrawRectangle(position.x, position.y, width, height, bg);
    DrawTextEx(font, text.c_str(), text_position, fontSize, fontSize/4, fg);
}