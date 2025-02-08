#include "beam/objects/text.h"
#include "beam/scene/scene.h"
#include "beam/scene/scene_manager.h"
#include <iostream>
#include <memory>

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

  void update(float, SharedManager manager) override {
    if (manager->isKeyPressed(KEY_ENTER)) {
      auto scene_manager = parentAs<SceneManager>();
      scene_manager->switchToScene("game", manager);
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