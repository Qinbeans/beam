#include "beam/objects/message_box.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

namespace {
// raygui's list-based controls take a single string with items separated by
// ';' rather than an array.
std::string joinItems(const std::vector<std::string> &items) {
  std::string joined;
  for (size_t i = 0; i < items.size(); ++i) {
    if (i > 0) {
      joined += ';';
    }
    joined += items[i];
  }
  return joined;
}
} // namespace

const Rectangle MessageBox::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void MessageBox::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  std::string joined = joinItems(buttons);
  // GuiMessageBox only writes btnActive on the frame a press occurs: 0 for
  // the window's own close button, i + 1 for buttons[i]. Its return value
  // (RESULT_PRESSED) tells us whether that happened this frame at all, so we
  // don't need to track edge-detection state ourselves.
  int prevBaseNormal = GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL);
  GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(STATUSBAR, TEXT_COLOR_NORMAL, ColorToInt(fg));

  int btnActive = -1;
  int result = GuiMessageBox(getBounds(), title.c_str(), message.c_str(),
                              joined.c_str(), &btnActive);

  GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(STATUSBAR, TEXT_COLOR_NORMAL, prevTextNormal);

  // Only a press on one of `buttons` (btnActive >= 1) maps to a valid index
  // into the buttons list; the window's close button (btnActive == 0) has no
  // corresponding entry, so it does not update lastButton or fire onButton.
  if (result == RESULT_PRESSED && btnActive >= 1) {
    lastButton = btnActive - 1;
    if (onButtonCallback) {
      onButtonCallback(this, manager, lastButton);
    }
  }
}

void MessageBox::setTitle(const std::string &title) { this->title = title; }

void MessageBox::setMessage(const std::string &message) { this->message = message; }

void MessageBox::setButtons(const std::vector<std::string> &buttons) { this->buttons = buttons; }

void MessageBox::setPosition(Vector2 position) { this->position = position; }

void MessageBox::setSize(Vector2 size) { this->size = size; }

void MessageBox::setBgColor(Color color) { bg = color; }

void MessageBox::setFgColor(Color color) { fg = color; }

const std::string &MessageBox::getTitle() const { return title; }

const std::string &MessageBox::getMessage() const { return message; }

const std::vector<std::string> &MessageBox::getButtons() const { return buttons; }

Vector2 MessageBox::getPosition() const { return position; }

Vector2 MessageBox::getSize() const { return size; }

int MessageBox::getLastButton() const { return lastButton; }

Color MessageBox::getBgColor() const { return bg; }

Color MessageBox::getFgColor() const { return fg; }

void MessageBox::onButton(std::function<void(MessageBox *, SharedManager, int)> callback) {
  onButtonCallback = callback;
}

} // namespace beam
