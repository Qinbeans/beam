#ifndef DEBUG_H
#define DEBUG_H

#include <fstream>
#include <string>

class Debug: public std::ofstream {
public:
    Debug() {
        open("debug.log");
    }

    ~Debug() {
        close();
    }
};

#endif