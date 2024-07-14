#include "state.h"
#include <iostream>
#include <memory>

State::State(int id, string name, string description, Vector2 position, Vector2 size, Color fg, Color bg, Padding padding, float spacing, Font font): Frame(position, size, fg, bg, padding) {
    this->state_id = id;
    this->spacing = spacing;
    this->name = name;
    this->description = description;
    this->text_size = MeasureTextEx(font, description.c_str(), 20, 20.0/4);
    this->size.y += text_size.y;
    this->lastButtonPosition = {position.x, position.y + text_size.y};
    this->font = font;
}

void State::init() {
    // create exit button
    lastButtonPosition.y += spacing;
    TextButton* button = new TextButton(
        "Exit",
        20,
        lastButtonPosition,
        Theme::FG,
        Theme::BG,
        Theme::FG_ALT,
        Theme::BG_ALT,
        padding,
        font
    );
    button->OnClick([](TextButton* self) {
        self->kill();
    }).OnHover([](TextButton* self) {}).OnUnhover([](TextButton* self) {});
    button->setParent(shared_ptr<Node>(this));
    buttons.push_back(button);
    for (auto button: buttons) {
        button->init();
    }
}

void State::update() {
    for (auto button: buttons) {
        button->update();
    }
}

void State::draw() {
    DrawRectangle(position.x, position.y, size.x, size.y, bg);
    DrawRectangle(position.x + padding.left, position.y + padding.top, text_size.x + padding.left + padding.right, text_size.y + padding.top + padding.bottom, fg);
    DrawTextEx(font, description.c_str(), {position.x + padding.left * 2, position.y + padding.top * 2}, 20, 20.0/4, Theme::FG);
    for (auto button: buttons) {
        button->draw();
    }
}

void State::debug() {
    std::cout << "State[" << state_id << "] name(" << name << ") parent(" << parent->getId() << ")" << std::endl;
    for (auto button: buttons) {
        button->debug();
    }
}