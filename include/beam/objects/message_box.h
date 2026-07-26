#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <string>
#include <vector>

namespace beam {
class MessageBox : public GameObject {
protected:
  std::string title;
  std::string message;
  std::vector<std::string> buttons;
  Vector2 position;
  Vector2 size;
  int lastButton;

  std::function<void(MessageBox *, SharedManager, int)> onButtonCallback;

public:
  MessageBox(const std::string &title, const std::string &message,
             const std::vector<std::string> &buttons, Vector2 position,
             Vector2 size)
      : title(title), message(message), buttons(buttons), position(position),
        size(size), lastButton(-1) {}

  void draw(SharedManager) override;

  void setTitle(const std::string &title);
  void setMessage(const std::string &message);
  void setButtons(const std::vector<std::string> &buttons);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);

  const std::string &getTitle() const;
  const std::string &getMessage() const;
  const std::vector<std::string> &getButtons() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;
  int getLastButton() const;

  const Rectangle getBounds() const;

  void onButton(std::function<void(MessageBox *, SharedManager, int)> callback);
};
} // namespace beam
