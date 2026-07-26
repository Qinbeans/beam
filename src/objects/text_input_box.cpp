#include "beam/objects/text_input_box.h"
#include "beam/objects/game_object.h"
#include "raygui.h"
#include <cstdio>

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

// GuiTextInputBox edits a fixed-size C buffer in place; this bounds how long
// a beam TextInputBox's content can grow.
constexpr int kTextInputBoxBufferSize = 256;
} // namespace

const Rectangle TextInputBox::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void TextInputBox::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  std::string joined = joinItems(buttons);

  char buffer[kTextInputBoxBufferSize];
  std::snprintf(buffer, sizeof(buffer), "%s", content.c_str());

  // GuiTextInputBox only writes btnActive on the frame a press occurs: 0 for
  // the window's own close button, i + 1 for buttons[i]. Its return value
  // (RESULT_PRESSED) tells us whether that happened this frame at all, so we
  // don't need to track edge-detection state ourselves. Text edit mode is
  // owned entirely by raygui's internal static state (see the class-level
  // warning), so we never pass/track an editMode flag of our own here.
  int btnActive = -1;
  int result = GuiTextInputBox(getBounds(), title.c_str(), message.c_str(),
                                buffer, sizeof(buffer), joined.c_str(),
                                &btnActive, &secretView);

  content = buffer;

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

void TextInputBox::setTitle(const std::string &title) { this->title = title; }

void TextInputBox::setMessage(const std::string &message) { this->message = message; }

void TextInputBox::setContent(const std::string &content) { this->content = content; }

void TextInputBox::setButtons(const std::vector<std::string> &buttons) { this->buttons = buttons; }

void TextInputBox::setSecretView(bool secretView) { this->secretView = secretView; }

void TextInputBox::setPosition(Vector2 position) { this->position = position; }

void TextInputBox::setSize(Vector2 size) { this->size = size; }

const std::string &TextInputBox::getTitle() const { return title; }

const std::string &TextInputBox::getMessage() const { return message; }

const std::string &TextInputBox::getContent() const { return content; }

const std::vector<std::string> &TextInputBox::getButtons() const { return buttons; }

bool TextInputBox::isSecretView() const { return secretView; }

Vector2 TextInputBox::getPosition() const { return position; }

Vector2 TextInputBox::getSize() const { return size; }

int TextInputBox::getLastButton() const { return lastButton; }

void TextInputBox::onButton(std::function<void(TextInputBox *, SharedManager, int)> callback) {
  onButtonCallback = callback;
}

} // namespace beam
