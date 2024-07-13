#include <iostream>
#include <raylib.h>
#include "event/event.h"
#include "event/utils.h"
#include "game_objects/text_button.h"
#include "event/object.h"
#include "game_objects/state.h"
#include "game_objects/state_man.h"
#include "game_objects/text.h"

int main(void) {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    // used to switch between game frames
    shared_ptr<int> state_tracker = std::make_shared<int>(0);
    
    InitWindow(screenWidth, screenHeight, "My Game");

    SetTargetFPS(120);

    // Font font = LoadFont("assets/fonts/Hack-Regular.ttf");
    Font font = LoadFontEx("assets/fonts/Hack-Regular.ttf", 20, 0, 250);

    // =============================== Game Objects ===============================
    Event root(
        "Root"
    );

    Manager manager(
        state_tracker
    );

    State state1(
        0,
        "Main Menu",
        "Welcome to my Dungeon!",
        Vector2{10, 10},
        Vector2{400,100},
        Theme::BG,
        Theme::PRIMARY,
        Padding{2, 10, 2, 10},
        25,
        font
    );

    State state2(
        1,
        "Play",
        "You are in a dark room.",
        Vector2{10, 10},
        Vector2{400,100},
        Theme::BG,
        Theme::PRIMARY,
        Padding{2, 10, 2, 10},
        25,
        font
    );

    State state3a(
        2,
        "Look around",
        "You see a table with\na map on it and a door\nat the end of the room.",
        Vector2{10, 10},
        Vector2{400,100},
        Theme::BG,
        Theme::PRIMARY,
        Padding{2, 10, 2, 10},
        25,
        font
    );

    State state3b(
        3,
        "Panic",
        "You panic and run\naround the room.",
        Vector2{10, 10},
        Vector2{400,100},
        Theme::BG,
        Theme::PRIMARY,
        Padding{2, 10, 2, 10},
        25,
        font
    );

    State state4a(
        4,
        "Look at the map",
        "The map shows a path\nleading somewhere.\n\"Forward, left, right,\nright, left, forward\"",
        Vector2{10, 10},
        Vector2{400,100},
        Theme::BG,
        Theme::PRIMARY,
        Padding{2, 10, 2, 10},
        25,
        font
    );

    State state4b(
        5,
        "Open the door",
        "You enter a corridor,\nthe door closes behind you.\nThere's a fork in the road:\nleft or right?",
        Vector2{10, 10},
        Vector2{400,100},
        Theme::BG,
        Theme::PRIMARY,
        Padding{2, 10, 2, 10},
        25,
        font
    );

    state1 << &state2;

    state2 << &state3a << &state3b;
    
    state3a << &state4a << &state4b;

    state3b << &state2;

    manager << &state1 << &state2 << &state3a << &state3b << &state4a << &state4b;

    root << &manager;
    // =============================== Game Objects ===============================

    root.init();

    try {
        while (!WindowShouldClose()) {
            // Update
            root.update();

            BeginDrawing();
            ClearBackground(BLACK);

            root.draw();

            EndDrawing();
            if (!root.isRunning()) break;
        }
    } catch (NodeException e) {
        std::cerr << e.what() << std::endl;
    }

    // De-Initialization
    UnloadFont(font); // Unload font to free memory
    CloseWindow();

    return 0;
}