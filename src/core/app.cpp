
// App.cpp
#include "beam/core/app.h"

namespace beam {

App::App(const std::string &windowTitle, int windowWidth, int windowHeight,
         int fps)
    : title(windowTitle), width(windowWidth), height(windowHeight),
      targetFPS(fps) {
  InitWindow(width, height, title.c_str());
  SetTargetFPS(targetFPS);

  rootEvent = std::make_shared<Event>("Root");
  sceneManager = std::make_shared<SceneManager>();
  *rootEvent << sceneManager;
}

App::~App() { CloseWindow(); }

Event &App::getEvent() { return *rootEvent; }

SceneManager &App::getSceneManager() { return *sceneManager; }

void App::run() {
  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();

    // Update
    rootEvent->update(deltaTime);

    // Draw
    BeginDrawing();
    ClearBackground(RAYWHITE);

    rootEvent->draw();

    EndDrawing();
  }
}

} // namespace beam