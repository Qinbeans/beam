#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>

namespace beam {
class Button : public GameObject {
protected:
  std::string text;
  float fontSize;
  float fontSpacing;
  Vector2 position;
  Vector2 size;
  Padding padding;
  Color bg;
  Color fg;
  Color bgHover;
  Color fgHover;
  std::string fontName;
  std::function<void(float, Button *, SharedManager)> updateCallback;
  std::function<void(Button *, SharedManager)> hoverCallback;
  std::function<void(Button *, SharedManager)> clickCallback;

public:
  /**
   * @brief Construct a new Button object
   */
  Button(const std::string &text, float fontSize, float fontSpacing, Vector2 position,
         Vector2 size, Padding padding, Color bg, Color fg, Color bgHover,
         Color fgHover, const std::string &fontName = "default")
      : text(text), fontSize(fontSize), fontSpacing(fontSpacing), position(position), size(size),
        padding(padding), bg(bg), fg(fg), bgHover(bgHover), fgHover(fgHover),
        fontName(fontName), updateCallback(nullptr), hoverCallback(nullptr),
        clickCallback(nullptr) {}

  void draw(SharedManager) override;
  void update(float, SharedManager) override;

  void setText(const std::string &text);
  void setFontName(const std::string &fontName);
  void setFontSize(float fontSize);
  void setFontSpacing(float fontSpacing);
  void setPosition(Vector2 position);
  void setSize(Vector2 size);
  void setPadding(Padding padding);
  void setBgColor(Color color);
  void setFgColor(Color color);
  void setBgHoverColor(Color color);
  void setFgHoverColor(Color color);

  const Rectangle getBounds() const;

  /**
   * @brief Get how big the button is on screen
   *
   * The size it was given plus its padding: the rectangle that is drawn,
   * hovered and clicked, and the same dimensions getBounds() reports.
   *
   * Deliberately not the Vector2 handed to setSize(). That one is the box the
   * label sits in, and a column of buttons laid out with it stacks them closer
   * together than they are drawn -- neighbours overlap by the padding, both are
   * live in the overlap, and a click there goes to whichever is tested first.
   * getPadding() recovers what setSize() was given.
   */
  Vector2 getSize() const;

  /**
   * @brief Get the space kept around the button's label
   */
  Padding getPadding() const;

  bool isHovered() const;
  bool isClicked() const;

  void onUpdate(std::function<void(float, Button *, SharedManager)> callback);
  void onHover(std::function<void(Button *, SharedManager)> callback);
  void onClick(std::function<void(Button *, SharedManager)> callback);
};
} // namespace beam