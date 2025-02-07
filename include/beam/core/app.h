// App.hpp
#pragma once

#include "beam/scene/scene_manager.h"
#include "event.h"

namespace beam {

class App {
private:
  std::string title;
  int width;
  int height;
  int targetFPS;
  std::shared_ptr<Event> rootEvent;
  std::shared_ptr<SceneManager> sceneManager;

public:
  App(const std::string &windowTitle, int windowWidth, int windowHeight,
      int fps);
  ~App();

  Event &getEvent();
  SceneManager &getSceneManager();
  void run();
};

} // namespace beam
