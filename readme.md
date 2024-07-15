# Game 2 - __The Second Project in Ryan's Course__

This teaches ideas ranging from inheritance to polymorphism. The idea is to get the student accustomed to the idea of creating a class that inherits from another class, and then overriding a method in the child class. In other words I want you to have fun while still being productive.

## Building

`make` can be used on all platforms to build the project.

```bash
make
```

For those who know how to use `cmake`, you can use the `CMakeLists.txt` file to generate a project file for your favorite IDE.

```bash
cmake -B build
cmake --build build
# or
cmake --build build --config Release
```

## Visualizing the project

There's a directory named `scripts` containing a Jupiter notebook that can be used to visualize the project. This will produce a graph of the States/Scenarios.

## The Game

The game already has some basic examples of how to get started. There's one instance of something called `Text` which drags a piece of text across the screen. You must create your own game that achieves what we did in the previous project, but with 2D graphics.

## The Rules

There are no rules. Just have fun. The only thing I ask is that you don't copy and paste the code. I want you to understand what you're doing. If you don't understand something, ask me. I'm here to help. I might say a question is stupid, but I'll still answer it. I promise.

## Example

```cpp
#include <raylib.h>
#include "./event/event.h"
#include "./game_objects/text.h"

int main() {
    InitWindow(800, 600, "Game 2");

    SetTargetFPS(60);

    Event root("Root");
    Text text("Hello, World!", 100, 100);
    Text text2("Hello, World!", 100, 200);

    // event loads in 2 text objects
    root << &text << &text2;

    // boilerplate code
    while (!WindowShouldClose()) {
        // updates should be called before drawing
        root.update();

        // set the canvas up
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // draw the objects
        root.draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
```

## Basic documentation

### Event

The `Event` class is used to handle the game loop, abstracting the process of updating and drawing objects.

#### __Constructor:__

```cpp
Event(std::string name);
```

A constructor serves the purpose of initializing the `Event` object with a name.

#### __Methods:__

#### *update*

```cpp
void update();
```

This method is used to update all the objects that are attached to the `Event` object.

#### *draw*

```cpp
void draw();
```

This method is used to draw all the objects that are attached to the `Event` object.

#### *operator<<*

```cpp
Event& operator<<(Object* object);
```

This method is used to attach a `Object` to the `Event` object.

### Object

The `Object` class is used to create objects that can be attached to the `Event` object. The way they operate is up to you. The `Object` class is an abstract class, meaning that it cannot be instantiated. You must create a class that inherits from `Object` and override the `update` and `draw` methods. An example of this is the `Text` class.

#### __Constructor:__

```cpp
Object();
```

A constructor serves the purpose of initializing the `Object` with a name, and a position.

#### __Methods:__

#### *update*

```cpp
virtual void update();
```

This method is used to update the object.

#### *draw*

```cpp
virtual void draw();
```

This method is used to draw the object.

### Text

The `Text` class is used to create text objects that can be attached to the `Event` object. The general action it performs is sliding across the screen. The `Text` class inherits from the `Object` class.

#### __Constructor:__

```cpp
Text(std::string name, int x, int y);
```

A constructor serves the purpose of initializing the `Text` object with a name, and a position.

#### __Methods:__

#### *update*

```cpp
void update();
```

This method is used to update the object.

#### *draw*

```cpp
void draw();
```

This method is used to draw the object.
