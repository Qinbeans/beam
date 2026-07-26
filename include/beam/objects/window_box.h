#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace beam {
class WindowBox : public GameObject {
protected:
  std::string title;
  Vector2 position;
  Vector2 size;
  Color bg;
  Color fg;

  std::vector<std::shared_ptr<GameObject>> buffer;

  std::function<void(WindowBox *, SharedManager)> closeCallback;

public:
  WindowBox(const std::string &title, Vector2 position, Vector2 size,
            Color bg = LIGHTGRAY, Color fg = DARKGRAY)
      : GameObject("WindowBox"), title(title), position(position), size(size),
        bg(bg), fg(fg) {}

  template <typename T> WindowBox &operator<<(std::shared_ptr<T> child) {
    static_assert(std::is_base_of<Node, T>::value,
                  "Child must be derived from Node");
    buffer.push_back(child);
    child->setParent(shared_from_this());
    return *this;
  }

  void init(SharedManager) override;
  void draw(SharedManager) override;
  void update(float, SharedManager) override;

  void setTitle(const std::string &title);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);
  void setBgColor(Color color);
  void setFgColor(Color color);

  const std::string &getTitle() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;
  Color getBgColor() const;
  Color getFgColor() const;

  const Rectangle getBounds() const;

  template <typename T> T *getChildAs(int index) {
    return dynamic_cast<T *>(buffer[index].get());
  }

  void onClose(std::function<void(WindowBox *, SharedManager)> callback);
};
} // namespace beam
