#include "beam/objects/button.h"
#include "beam/objects/frame.h"
#include "beam/objects/text.h"
#include "beam/scene/scene.h"
#include "beam/scene/scene_manager.h"
#include "raylib.h"
#include <iostream>
#include <memory>

using namespace beam;

class MenuScene : public Scene {
public:
  MenuScene() : Scene("menu") {}

  void init(SharedManager manager) override {
    int exampleState = manager->getState<int>("example state");
    auto text = std::make_shared<Text>("Menu Scene", 400, 300, 40);
    auto stateText = std::make_shared<Text>(
        "State: " + std::to_string(exampleState), 400, 350, 20);

    auto menuFrame = std::make_shared<Frame>("Menu Frame", Vector2{10, 10},
                                             Rectangle{10, 10, 140, 600}, BLUE);

    auto exitButton = std::make_shared<Button>(
        "Exit", 20, Vector2{10, 10}, Vector2{100, 25}, Padding{10, 10, 10, 10},
        RED, WHITE, MAROON, WHITE);

    exitButton->onClick([&](Button *, SharedManager manager) {
      std::cout << "Exiting game" << std::endl;
      manager->close();
    });

    *menuFrame << exitButton;

    *this << menuFrame << text << stateText;
    Scene::init(manager);
  }

  void update(float dt, SharedManager manager) override {
    if (manager->isKeyPressed(KEY_ENTER)) {
      auto scene_manager = parentAs<SceneManager>();
      scene_manager->switchToScene("game", manager);
      parent.reset();
    }
    Scene::update(dt, manager);
  }

  void onEnter(SharedManager manager) override {
    int exampleState = manager->getState<int>("example state");
    manager->setState("example state", exampleState + 1);
    std::cout << "Entering Menu Scene" << std::endl;
  }

  void onExit(SharedManager) override {
    std::cout << "Exiting Menu Scene" << std::endl;
  }
};