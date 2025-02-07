// Description: A basic game example using the game engine.

#include "beam/core/app.h"
#include "beam/objects/text.h"
#include "raylib.h"
#include <iostream>

using namespace beam;

class MenuScene : public Scene {
public:
  MenuScene() : Scene("menu") {
    auto text = std::make_shared<Text>("Menu Scene", 400, 300, 40);
    *this << text;
  }

  void update(float _deltaTime) override {
    if (IsKeyPressed(KEY_ENTER)) {
      auto parent = getParent().lock();
      auto manager = dynamic_cast<SceneManager *>(parent.get());
      manager->switchToScene("game");
      parent.reset();
    }
  }

  void onEnter() override { std::cout << "Entering Menu Scene" << std::endl; }

  void onExit() override { std::cout << "Exiting Menu Scene" << std::endl; }
};

class GameScene : public Scene {
public:
  GameScene() : Scene("game") {
    auto text = std::make_shared<Text>("Game Scene", 400, 300, 40);
    *this << text;
  }

  void onEnter() override { std::cout << "Entering Game Scene" << std::endl; }

  void onExit() override { std::cout << "Exiting Game Scene" << std::endl; }
};

int main() {
  App app("Game Engine Demo", 800, 600, 60);

  // Set up scenes
  auto &sceneManager = app.getSceneManager();
  sceneManager << std::make_shared<MenuScene>();
  sceneManager << std::make_shared<GameScene>();

  // Switch to initial scene
  sceneManager.switchToScene("menu");

  // Add standalone text to root event
  Event &root = app.getEvent();
  auto text = std::make_shared<Text>("Press ESC to exit", 10, 10, 20);
  root << text;

  app.run();
  return 0;
}