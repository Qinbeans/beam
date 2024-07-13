#include "event.h"
#include "node.h"
#include <iostream>

Event::Event() {
    id = node_id++;
    name = "Event";
    parent = nullptr;
}

Event::Event(string name) {
    id = node_id++;
    this->name = name;
    parent = nullptr;
}

Event::Event(string name, vector<Object*> objects) {
    id = node_id++;
    this->name = name;
    this->objects = objects;
    parent = nullptr;
}

Event::Event(const Event& event) {
    id = node_id++;
    name = event.name;
    objects = event.objects;
    parent = event.parent;
}

void Event::kill() {
    running = false;
}

void Event::init() {
    if (!initialized) {
        initialized = true;
    }
    running = true;
    for (Object* object : objects) {
        object->init();
    }
}

void Event::update() {
    if (!initialized) {
        throw NodeException(getId(),"Event not initialized: update() called before init()");
    }
    for (Object* object : objects) {
        object->update();
    }
}

void Event::draw() {
    if (!initialized) {
        throw NodeException(getId(),"Event not initialized: draw() called before init()");
    }
    for (Object* object : objects) {
        object->draw();
    }
}