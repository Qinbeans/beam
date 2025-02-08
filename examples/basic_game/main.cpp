// Description: A basic game example using the game engine.

#include "beam/core/app.h"
#include "beam/core/manager.h"
#include "beam/objects/button.h"
#include "game_scene.h"
#include "menu_scene.h"
#include "raylib.h"
#include <iostream>

using namespace beam;

std::shared_ptr<Scene> createMenuScene(SharedManager manager) {
  return std::make_shared<MenuScene>(manager);
}

std::shared_ptr<Scene> createGameScene(SharedManager manager) {
  return std::make_shared<GameScene>(manager);
}

int main() {
  App app("Game Engine Demo", 800, 600, 60, true);

  auto manager = app.getManager();

  manager->setState<int>("example state", 0);

  // Set up scenes
  auto &sceneManager = app.getSceneManager();
  sceneManager << createMenuScene(manager) << createGameScene(manager);

  // Switch to initial scene
  sceneManager.switchToScene("menu", manager);

  // Add standalone text to root event
  Event &root = app.getEvent();

  // create an exit button
  auto exitButton = std::make_shared<Button>(
      "Exit", 20, Vector2{700, 500}, Vector2{100, 50}, Padding{10, 10, 10, 10},
      RED, WHITE, MAROON, WHITE);
  exitButton->onClick(
      [&](Button *, SharedManager manager) { manager->close(); });

  root << exitButton;

  app.run();

  int exampleState = manager->getState<int>("example state");
  std::cout << "Final state: " << exampleState << std::endl;
  return 0;
}