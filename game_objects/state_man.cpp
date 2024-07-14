#include "state_man.h"

void Manager::init() {
    if (initialized) return;
    initialized = true;
    for (shared_ptr<Frame> state : states) {
        state->init();
    }
}

void Manager::update() {
    if (!initialized) {
        throw NodeException(getId(),"Manager not initialized: update() called before init()");
    }
    states[*state]->update();
}

void Manager::draw() {
    if (!initialized) {
        throw NodeException(getId(),"Manager not initialized: draw() called before init()");
    }
    states[*state]->draw();
}

void Manager::debug() {
    if (!initialized) {
        throw NodeException(getId(),"Manager not initialized: debug() called before init()");
    }
    std::cout << "Manager[" << id << "] state(" << *state << ") parent(" << parent->getId() << ")" << std::endl;
    for (shared_ptr<Frame> state : states) {
        state->debug();
    }
}