#ifndef STATE_MAN_H
#define STATE_MAN_H

#include <memory>
#include <vector>
#include <string>
#include <raylib.h>
#include "frame.h"

class State;

class Manager: public Object {
    protected:
        std::vector<shared_ptr<Frame>> states;
        shared_ptr<int> state;
    public:
        Manager(shared_ptr<int> state): state(state) {
            id = node_id++;
        }
        int getState() { return *state; }
        void setState(int state) {
            if (state < states.size()) {
                *this->state = state;
            }
        }
        void init() override;
        void update() override;
        void draw() override;
        void debug(Debug &debug) override;
        friend Manager& operator<<(Manager& manager, Frame* frame) {
            manager.states.push_back(shared_ptr<Frame>(frame));
            frame->setParent(shared_ptr<Node>(&manager));
            return manager;
        }
};

#endif // STATE