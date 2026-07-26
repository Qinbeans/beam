#include "beam/objects/scroll_panel.h"
#include "beam/objects/game_object.h"
#include "raygui.h"
#include "rlgl.h"

namespace beam {

const Rectangle ScrollPanel::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void ScrollPanel::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  int prevBackground = GuiGetStyle(DEFAULT, BACKGROUND_COLOR);
  int prevBorderNormal = GuiGetStyle(LISTVIEW, BORDER_COLOR_NORMAL);
  int prevBorderFocused = GuiGetStyle(LISTVIEW, BORDER_COLOR_FOCUSED);
  int prevBorderPressed = GuiGetStyle(LISTVIEW, BORDER_COLOR_PRESSED);
  GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(bg));
  GuiSetStyle(LISTVIEW, BORDER_COLOR_NORMAL, ColorToInt(fg));
  GuiSetStyle(LISTVIEW, BORDER_COLOR_FOCUSED, ColorToInt(fg));
  GuiSetStyle(LISTVIEW, BORDER_COLOR_PRESSED, ColorToInt(fg));

  GuiScrollPanel(getBounds(), nullptr, content, &scroll, &view);

  GuiSetStyle(DEFAULT, BACKGROUND_COLOR, prevBackground);
  GuiSetStyle(LISTVIEW, BORDER_COLOR_NORMAL, prevBorderNormal);
  GuiSetStyle(LISTVIEW, BORDER_COLOR_FOCUSED, prevBorderFocused);
  GuiSetStyle(LISTVIEW, BORDER_COLOR_PRESSED, prevBorderPressed);

  BeginScissorMode((int)view.x, (int)view.y, (int)view.width,
                    (int)view.height);
  rlPushMatrix();
  rlTranslatef(position.x - scroll.x, position.y - scroll.y, 0);
  for (const auto &child : buffer) {
    child->draw(manager);
  }
  rlPopMatrix();
  EndScissorMode();
}

void ScrollPanel::init(SharedManager manager) {
  GameObject::init(manager);
  for (const auto &child : buffer) {
    child->init(manager);
  }
}

void ScrollPanel::update(float delta, SharedManager manager) {
  GameObject::update(delta, manager);
  for (const auto &child : buffer) {
    child->update(delta, manager);
  }
}

void ScrollPanel::setPosition(Vector2 position) { this->position = position; }

void ScrollPanel::setSize(Vector2 size) { this->size = size; }

void ScrollPanel::setContent(Rectangle content) { this->content = content; }

void ScrollPanel::setScroll(Vector2 scroll) { this->scroll = scroll; }

void ScrollPanel::setBgColor(Color color) { bg = color; }

void ScrollPanel::setFgColor(Color color) { fg = color; }

Vector2 ScrollPanel::getPosition() const { return position; }

Vector2 ScrollPanel::getSize() const { return size; }

Rectangle ScrollPanel::getContent() const { return content; }

Vector2 ScrollPanel::getScroll() const { return scroll; }

Rectangle ScrollPanel::getView() const { return view; }

Color ScrollPanel::getBgColor() const { return bg; }

Color ScrollPanel::getFgColor() const { return fg; }

} // namespace beam
