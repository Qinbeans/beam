#pragma once

#include "beam/objects/text.h"
#include "beam/scene/scene.h"
#include <iostream>

using namespace beam;

class GameScene : public Scene {
public:
  GameScene() : Scene("game") {}

  void init(SharedManager manager) override {
    int exampleState = manager->getState<int>("example state");
    auto text = std::make_shared<Text>("Game Scene", 400, 300, 40);
    auto stateText = std::make_shared<Text>(
        "State: " + std::to_string(exampleState), 400, 350, 20);
    *this << text << stateText;
    Scene::init(manager);
  }

  void onEnter(SharedManager manager) override {
    int exampleState = manager->getState<int>("example state");
    manager->setState("example state", exampleState + 1);
    manager->setBackgroundColor(BLUE);
    std::cout << "Entering Game Scene" << std::endl;
  }

  void onExit(SharedManager) override {
    std::cout << "Exiting Game Scene" << std::endl;
  }
};