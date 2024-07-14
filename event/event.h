#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>
#include "object.h"

using std::vector;
using std::string;

class Event: public Node{
    private:
        vector<Object*> objects;
        string name;
        bool running;
    public:
        Event();
        Event(string name);
        Event(string name, vector<Object*> objects);
        Event(const Event& event);
        
        friend Event& operator<<(Event& event, Object* object) {
            event.objects.push_back(object);
            // Ties the object to the event
            object->setParent(shared_ptr<Node>(&event));
            return event;
        }
        string getType() { return "Event"; };
        bool isRunning() { return running; }
        void kill() override;
        void init() override;
        void update() override;
        void draw() override;
        void debug() override;
};

#endif // EVENT_H