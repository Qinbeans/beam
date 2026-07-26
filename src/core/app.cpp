
// App.cpp
#include "beam/core/app.h"
#include "beam/core/manager.h"
#include "raylib.h"
#include <memory>

namespace beam {

App::App(const std::string &windowTitle, int windowWidth, int windowHeight,
         int fps, bool fullscreen)
    : title(windowTitle), width(windowWidth), height(windowHeight),
      targetFPS(fps) {
  InitWindow(width, height, title.c_str());
  SetTargetFPS(targetFPS);

  if (fullscreen) {
    ToggleFullscreen();
  }

  SetExitKey(KEY_NULL);

  rootEvent = std::make_shared<Event>("Root");
  sceneManager = std::make_shared<SceneManager>();
  manager = std::make_shared<Manager>();
  *rootEvent << sceneManager;
}

App::~App() { CloseWindow(); }

void App::init() { rootEvent->init(manager); }

Event &App::getEvent() { return *rootEvent; }

SceneManager &App::getSceneManager() { return *sceneManager; }

void App::run() {
  while (!WindowShouldClose() && !manager->closed()) {
    float deltaTime = GetFrameTime();

    // Update
    rootEvent->update(deltaTime, manager);

    // Draw
    BeginDrawing();
    ClearBackground(manager->getBackgroundColor());

    rootEvent->draw(manager);

    EndDrawing();
  }
}

SharedManager App::getManager() const { return manager; }

} // namespace beam