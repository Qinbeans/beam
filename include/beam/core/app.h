// App.hpp
#pragma once

#include "beam/core/asset.h"
#include "beam/core/event.h"
#include "beam/core/manager.h"
#include "beam/scene/scene_manager.h"
#include <memory>

namespace beam {

class App {
private:
  std::string title;
  int width;
  int height;
  int targetFPS;
  std::shared_ptr<Event> rootEvent;
  std::shared_ptr<SceneManager> sceneManager;
  SharedManager manager;

public:
  App(const std::string &windowTitle, int windowWidth, int windowHeight,
      int fps);
  ~App();

  Event &getEvent();
  SceneManager &getSceneManager();
  void run();

  // getters
  SharedManager getManager() const;
};

} // namespace beam
