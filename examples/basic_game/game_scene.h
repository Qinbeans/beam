#pragma once

#include "beam/objects/button.h"
#include "beam/objects/frame.h"
#include "beam/objects/text.h"
#include "beam/scene/scene.h"
#include "raylib.h"
#include <iostream>

using namespace beam;

class GameScene : public Scene {
public:
  GameScene() : Scene("game") {}

  void init(SharedManager manager) override {
    int exampleState = manager->getState<int>("example state");
    auto text = std::make_shared<Text>("Game Scene", 400, 300, "default", 40);
    auto stateText = std::make_shared<Text>(
        "State: " + std::to_string(exampleState), 400, 350, "default", 20);

    auto menuFrame = std::make_shared<Frame>("Menu Frame", Vector2{10, 10},
                                             Rectangle{10, 10, 140, 600}, BLUE);

    auto exitButton = std::make_shared<Button>(
        "Exit", 20, 1, Vector2{10, 10}, Vector2{100, 25}, Padding{10, 10, 10, 10},
        RED, WHITE, MAROON, WHITE);

    exitButton->onClick([&](Button *, SharedManager manager) {
      std::cout << "Exiting game" << std::endl;
      manager->close();
    });

    *menuFrame << exitButton;

    menuFrame->setActive(false);

    *this << menuFrame << text << stateText;
    manager->setState("example state", exampleState + 1);
    Scene::init(manager);
  }

  void update(float dt, SharedManager manager) override {
    if (manager->isKeyPressed(KEY_ESCAPE)) {
      auto frame = getChildAs<Frame>(0);
      frame->setActive(!frame->isActive());
    }
    Scene::update(dt, manager);
  }

  void onEnter(SharedManager manager) override {
    manager->setBackgroundColor(PURPLE);
    std::cout << "Entering Game Scene" << std::endl;
  }

  void onExit(SharedManager) override {
    std::cout << "Exiting Game Scene" << std::endl;
  }
};