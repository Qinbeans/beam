#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <memory>
#include <type_traits>
#include <vector>

namespace beam {
class ScrollPanel : public GameObject {
protected:
  Vector2 position;
  Vector2 size;
  Rectangle content;
  Vector2 scroll;
  Rectangle view;

  std::vector<std::shared_ptr<GameObject>> buffer;

public:
  ScrollPanel(Vector2 position, Vector2 size, Rectangle content)
      : position(position), size(size), content(content), scroll{0, 0},
        view{0, 0, 0, 0} {}

  template <typename T> ScrollPanel &operator<<(std::shared_ptr<T> child) {
    static_assert(std::is_base_of<Node, T>::value,
                  "Child must be derived from Node");
    buffer.push_back(child);
    child->setParent(shared_from_this());
    return *this;
  }

  void init(SharedManager) override;
  void draw(SharedManager) override;
  void update(float, SharedManager) override;

  void setPosition(Vector2);
  void setSize(Vector2);
  void setContent(Rectangle);
  void setScroll(Vector2);

  Vector2 getPosition() const;
  Vector2 getSize() const;
  Rectangle getContent() const;
  Vector2 getScroll() const;
  Rectangle getView() const;

  const Rectangle getBounds() const;
  template <typename T> T *getChildAs(int index) {
    return dynamic_cast<T *>(buffer[index].get());
  }
};
} // namespace beam
