#include "beam/objects/input.h"
#include "beam/objects/game_object.h"
#include "raygui.h"
#include <cstdio>

namespace beam {

namespace {
// GuiTextBox edits a fixed-size C buffer in place; this bounds how long a
// beam Input's content can grow.
constexpr int kInputBufferSize = 256;
} // namespace

void Input::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  Font font;
  if (manager->hasAsset(fontName)) {
    font = manager->getAsset<Font>(fontName);
  } else {
    font = GetFontDefault();
  }

  Font prevFont = GuiGetFont();
  int prevTextSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
  int prevTextSpacing = GuiGetStyle(DEFAULT, TEXT_SPACING);
  int prevBaseNormal = GuiGetStyle(TEXTBOX, BASE_COLOR_NORMAL);
  int prevTextNormal = GuiGetStyle(TEXTBOX, TEXT_COLOR_NORMAL);
  int prevBaseFocused = GuiGetStyle(TEXTBOX, BASE_COLOR_FOCUSED);
  int prevTextFocused = GuiGetStyle(TEXTBOX, TEXT_COLOR_FOCUSED);

  GuiSetFont(font);
  GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(fontSize));
  GuiSetStyle(DEFAULT, TEXT_SPACING, static_cast<int>(fontSpacing));
  GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, ColorToInt(bg));
  GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(fg));
  GuiSetStyle(TEXTBOX, BASE_COLOR_FOCUSED, ColorToInt(bgHover));
  GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(fgHover));

  // Show the placeholder only while empty and unfocused; GuiTextBox owns the
  // buffer while focused, so content is only written back in that case.
  const bool showPlaceholder = content.empty() && !focused;
  char buffer[kInputBufferSize];
  std::snprintf(buffer, sizeof(buffer), "%s",
                showPlaceholder ? placeholder.c_str() : content.c_str());

  GuiTextBox(getBounds(), buffer, sizeof(buffer), focused);

  if (!showPlaceholder) {
    content = buffer;
  }

  GuiSetFont(prevFont);
  GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
  GuiSetStyle(DEFAULT, TEXT_SPACING, prevTextSpacing);
  GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, prevBaseNormal);
  GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, prevTextNormal);
  GuiSetStyle(TEXTBOX, BASE_COLOR_FOCUSED, prevBaseFocused);
  GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, prevTextFocused);
}

void Input::update(float dt, SharedManager manager) {
  Button::update(dt, manager);
  if (!active) {
    return;
  }

  // GuiTextBox (called from draw()) owns cursor blinking and keystroke
  // handling internally while editMode/focused is true; only focus/blur
  // transitions are tracked here.
  if (isHovered() && manager->isMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
      !focused) {
    focused = true;
    if (focusCallback) {
      focusCallback(this, manager);
    }
  }

  if (!isHovered() && manager->isMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
      focused) {
    focused = false;
    if (blurCallback) {
      blurCallback(this, manager);
    }
  }
}

void Input::setContent(const std::string &content) { this->content = content; }

void Input::setFocused(bool focused) { this->focused = focused; }

const std::string &Input::getContent() const { return content; }

bool Input::isFocused() const { return focused; }

void Input::onFocus(std::function<void(Input *, SharedManager)> callback) {
  focusCallback = callback;
}

void Input::onBlur(std::function<void(Input *, SharedManager)> callback) {
  blurCallback = callback;
}

} // namespace beam