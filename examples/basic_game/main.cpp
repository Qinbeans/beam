// Description: A basic game example using the game engine.

#include "beam/core/app.h"
#include "beam/core/manager.h"
#include "game_scene.h"
#include "menu_scene.h"
#include <iostream>

using namespace beam;

std::shared_ptr<Scene> createMenuScene() {
  return std::make_shared<MenuScene>();
}

std::shared_ptr<Scene> createGameScene() {
  return std::make_shared<GameScene>();
}

int main() {
  App app("Game Engine Demo", 800, 600, 60);

  auto manager = app.getManager();

  manager->setState<int>("example state", 0);

  // Set up scenes
  auto &sceneManager = app.getSceneManager();
  sceneManager << createMenuScene() << createGameScene();

  // Switch to initial scene
  sceneManager.switchToScene("menu", manager);

  app.init();

  app.run();

  int exampleState = manager->getState<int>("example state");
  std::cout << "Final state: " << exampleState << std::endl;
  return 0;
}