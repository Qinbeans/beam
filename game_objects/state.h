#ifndef STATE_H
#define STATE_H

#include "text_button.h"
#include "frame.h"
#include <iostream>
#include <string>
#include "../event/utils.h"
#include "state_man.h"

const int STATE_OFFSET = 3;

// A special frame that generates buttons and text given a scenario
class State: public Frame {
    private:
        int state_id;
        Vector2 lastButtonPosition;
        vector<TextButton*> buttons;
        float spacing;
        string name;
        string description;
        Font font;
        Vector2 text_size;
    public:
        State(int id, string name, string description, Vector2 position, Vector2 size, Color fg, Color bg, Padding padding, float spacing, Font font = GetFontDefault());
        float getSpacing() const { return spacing; }
        State& setSpacing(float spacing) { this->spacing = spacing; return *this; }
        string getName() const { return name; }
        State& setName(string name) { this->name = name; return *this; }
        int getId() const override { return id; }
        State& setId(int id) override { this->id = id; return *this; }
        void init() override;
        void update() override;
        void draw() override;
        friend State& operator<<(State& state, State* state2) {
            // create a new button that sets the state to the given state by id
            state.lastButtonPosition.y += state.spacing;
            TextButton* button = new TextButton(
                state2->getName(),
                20,
                state.lastButtonPosition,
                Theme::FG,
                Theme::BG,
                Theme::FG_ALT,
                Theme::BG_ALT,
                state.padding,
                state.font
            );
            state.size.y += MeasureTextEx(state.font, state2->getName().c_str(), 20, 20.0/4).y;
            button->setParent(shared_ptr<Node>(&state));
            button->setId(state2->getId());
            button->OnClick([](TextButton* self) {
                shared_ptr<Manager> man = dynamic_pointer_cast<Manager>(self->getParent()->getParent());
                if (!man) {
                    throw NodeException(self->getId(), "Parent of parent of button is not a Manager");
                }
                man->setState(self->getId());
            }).OnHover([](TextButton* self) {}).OnUnhover([](TextButton* self) {});

            state.buttons.push_back(button);

            return state;
        }
};

#endif // STATE_H