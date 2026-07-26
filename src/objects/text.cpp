#include "beam/objects/text.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

Text::Text(const std::string &text, float x, float y, const std::string &fontName, float fontSize, float fontSpacing)
    : GameObject("Text"), content(text), fontSize(fontSize), fontSpacing(fontSpacing), fontName(fontName), color(BLACK) {
  position = {x, y};
}

void Text::draw(SharedManager manager) {
  if (active) {
    Font font;
    if (manager->hasAsset(fontName)) {
      font = manager->getAsset<Font>(fontName);
    } else {
      font = GetFontDefault();
    }

    Font prevFont = GuiGetFont();
    int prevTextSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    int prevTextSpacing = GuiGetStyle(DEFAULT, TEXT_SPACING);
    int prevTextColor = GuiGetStyle(LABEL, TEXT_COLOR_NORMAL);
    int prevAlignment = GuiGetStyle(LABEL, TEXT_ALIGNMENT);

    GuiSetFont(font);
    GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(fontSize));
    GuiSetStyle(DEFAULT, TEXT_SPACING, static_cast<int>(fontSpacing));
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(color));
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);

    // GuiLabel does not lay the string out in the rectangle it is handed: it
    // first insets it by the control's border width and text padding. Sizing
    // the rectangle to exactly the measured text therefore leaves the string
    // wider than the space it is given, and raygui responds by replacing the
    // tail of every line with an ellipsis. Grow the rectangle by that same
    // inset - and shift its origin back by it - so the text is laid out at the
    // full width it measured, starting exactly at this object's position.
    const float inset = static_cast<float>(GuiGetStyle(LABEL, BORDER_WIDTH) +
                                           GuiGetStyle(LABEL, TEXT_PADDING));
    const int width = GuiGetTextWidth(content.c_str());
    GuiLabel({position.x - inset, position.y - inset,
              static_cast<float>(width) + 2.0f * inset, fontSize + 2.0f * inset},
             content.c_str());

    GuiSetFont(prevFont);
    GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
    GuiSetStyle(DEFAULT, TEXT_SPACING, prevTextSpacing);
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, prevTextColor);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, prevAlignment);
  }
  GameObject::draw(manager);
}

void Text::setText(const std::string &text) { content = text; }

void Text::setColor(Color newColor) { color = newColor; }

void Text::setFontName(const std::string &fontName) { this->fontName = fontName; }

void Text::setFontSize(float size) { fontSize = size; }

void Text::setFontSpacing(float spacing) { fontSpacing = spacing; }

const std::string &Text::getText() const { return content; }

Color Text::getColor() const { return color; }

float Text::getFontSize() const { return fontSize; }

} // namespace beam