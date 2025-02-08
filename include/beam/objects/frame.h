#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <vector>

namespace beam {
class Frame : public GameObject {
private:
  Vector2 origin;
  Rectangle bound;
  Color bg;
  float rotation;

  std::vector<std::shared_ptr<GameObject>> buffer;

  std::function<void(float, SharedManager)> updateCallback;
  std::function<void(SharedManager)> drawCallback;

public:
  Frame(const std::string &name, Vector2 origin, Rectangle bound,
        Color bg = BLANK);

  template <typename T> Frame &operator<<(std::shared_ptr<T> child) {
    static_assert(std::is_base_of<Node, T>::value,
                  "Child must be derived from Node");
    buffer.push_back(child);
    child->setParent(shared_from_this());
    return *this;
  }

  void init(SharedManager) override;
  void draw(SharedManager) override;
  void update(float, SharedManager) override;

  void setOrigin(Vector2);
  void setBound(Rectangle);
  void setBackgroundColor(Color);
  void setRotation(float);

  Vector2 getOrigin() const;
  Rectangle getBound() const;
  Color getBackgroundColor() const;
  float getRotation() const;
  template <typename T> T *getChildAs(int index) {
    return dynamic_cast<T *>(buffer[index].get());
  }

  void onUpdate(std::function<void(float, SharedManager)>);
  void onDraw(std::function<void(SharedManager)>);
};
} // namespace beam