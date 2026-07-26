#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <string>
#include <vector>

namespace beam {
// GuiTextInputBox edits its text buffer through a static, function-internal
// textEditMode flag shared by every call site in the whole program (raygui's
// own documented warning), so only one beam::TextInputBox should be actively
// shown/edited at a time.
class TextInputBox : public GameObject {
protected:
  std::string title;
  std::string message;
  std::string content;
  std::vector<std::string> buttons;
  bool secretView;
  Vector2 position;
  Vector2 size;
  int lastButton;

  std::function<void(TextInputBox *, SharedManager, int)> onButtonCallback;

public:
  TextInputBox(const std::string &title, const std::string &message,
               const std::vector<std::string> &buttons, Vector2 position,
               Vector2 size)
      : title(title), message(message), content(""), buttons(buttons),
        secretView(false), position(position), size(size), lastButton(-1) {}

  void draw(SharedManager) override;

  void setTitle(const std::string &title);
  void setMessage(const std::string &message);
  void setContent(const std::string &content);
  void setButtons(const std::vector<std::string> &buttons);
  void setSecretView(bool secretView);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);

  const std::string &getTitle() const;
  const std::string &getMessage() const;
  const std::string &getContent() const;
  const std::vector<std::string> &getButtons() const;
  bool isSecretView() const;
  Vector2 getPosition() const;
  Vector2 getSize() const;
  int getLastButton() const;

  const Rectangle getBounds() const;

  void onButton(std::function<void(TextInputBox *, SharedManager, int)> callback);
};
} // namespace beam
