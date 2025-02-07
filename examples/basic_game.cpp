// Description: A basic game example using the game engine.

#include "beam/core/app.h"
#include "beam/core/manager.h"
#include "beam/objects/button.h"
#include "beam/objects/text.h"
#include "raylib.h"
#include <iostream>

using namespace beam;

class MenuScene : public Scene {
public:
  MenuScene(SharedManager manager) : Scene("menu") {
    int exampleState = manager->getState<int>("example state");
    manager->setState("example state", exampleState + 1);
    auto text = std::make_shared<Text>("Menu Scene", 400, 300, 40);
    auto stateText = std::make_shared<Text>(
        "State: " + std::to_string(exampleState), 400, 350, 20);
    *this << text << stateText;
  }

  void update(float, SharedManager managers) override {
    if (IsKeyPressed(KEY_ENTER)) {
      auto manager = parentAs<SceneManager>();
      manager->switchToScene("game", managers);
      parent.reset();
    }
  }

  void onEnter(SharedManager) override {
    std::cout << "Entering Menu Scene" << std::endl;
  }

  void onExit(SharedManager) override {
    std::cout << "Exiting Menu Scene" << std::endl;
  }
};

class GameScene : public Scene {
public:
  GameScene(SharedManager manager) : Scene("game") {
    int exampleState = manager->getState<int>("example state");
    manager->setState("example state", exampleState + 1);
    auto text = std::make_shared<Text>("Game Scene", 400, 300, 40);
    auto stateText = std::make_shared<Text>(
        "State: " + std::to_string(exampleState), 400, 350, 20);
    *this << text << stateText;
  }

  void onEnter(SharedManager) override {
    std::cout << "Entering Game Scene" << std::endl;
  }

  void onExit(SharedManager) override {
    std::cout << "Exiting Game Scene" << std::endl;
  }
};

int main() {
  App app("Game Engine Demo", 800, 600, 60);

  auto manager = app.getManager();

  manager->setState<int>("example state", 0);

  // Set up scenes
  auto &sceneManager = app.getSceneManager();
  sceneManager << std::make_shared<MenuScene>(manager);
  sceneManager << std::make_shared<GameScene>(manager);

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