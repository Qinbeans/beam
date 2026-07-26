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

  GuiScrollPanel(getBounds(), nullptr, content, &scroll, &view);

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

Vector2 ScrollPanel::getPosition() const { return position; }

Vector2 ScrollPanel::getSize() const { return size; }

Rectangle ScrollPanel::getContent() const { return content; }

Vector2 ScrollPanel::getScroll() const { return scroll; }

Rectangle ScrollPanel::getView() const { return view; }

} // namespace beam
