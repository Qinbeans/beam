#ifndef NODE_H
#define NODE_H

#include <exception>
#include <iostream>
#include <string>
#include <raylib.h>
#include <memory>

using std::string;
using std::exception;
using std::shared_ptr;

class NodeException : public exception {
    private:
        string message;
    public:
        NodeException(int id, const string& message) {
            this->message = "[" + std::to_string(id) + "] " + message;
        }
        const char* what() const noexcept override {
            return message.c_str();
        }
};

static int node_id = 0;

class Node {
    protected:
        int id;
        shared_ptr<Node> parent;
        Vector2 position;
        bool initialized;
    public:
        Node() : id(node_id++), parent(nullptr), position{0, 0}, initialized(false) {}

        virtual int getId() const {
            return id;
        }

        virtual Node& setId(int id) {
            this->id = id;
            return *this;
        }

        virtual string getType() const {
            return "Node";
        }

        virtual void init() {
            if (initialized) {
                return;
            }
            initialized = true;
        }

        virtual void update() {
            if (!initialized) {
                throw NodeException(getId(), getType() + " not initialized: update() called before init()");
            }
        }

        virtual void draw() {
            if (!initialized) {
                throw NodeException(getId(), getType() + " not initialized: draw() called before init()");
            }
        }

        virtual void kill() {
            if (parent == nullptr) {
                std::cout << "Node[" << id << "] has no parent" << std::endl;
            }
            parent->kill();
        }
        
        virtual Node& setParent(shared_ptr<Node> parent) {
            this->parent = parent;
            return *this;
        }

        virtual shared_ptr<Node> getParent() const {
            return parent;
        }

        virtual Vector2 getPosition() const {
            return position;
        }

        virtual void unsetParent() {
            parent = nullptr;
        }
        virtual void debug() {
            if (parent != nullptr) {
                std::cout << getType() << "[" << id << "] parent(" << parent->getId() << ")" << std::endl;
            } else {
                std::cout << getType() << "[" << id << "] has no parent" << std::endl;
            }
        }
};

#endif // NODE_H
