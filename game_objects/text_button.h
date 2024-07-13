#ifndef TEXT_BUTTON_H
#define TEXT_BUTTON_H

#include <string>
#include <exception>
#include <raylib.h>
#include "text.h"

class TextButton : public Text {
    private:
        // list of function pointers
        void (*onClick)(TextButton* self);
        void (*onHover)(TextButton* self);
        void (*onUnhover)(TextButton* self);
        bool isHovered;
        Color fgHover;
        Color bgHover;
    public:
        TextButton(): Text(), onClick(nullptr), onHover(nullptr), onUnhover(nullptr), isHovered(false), fgHover(BLANK), bgHover(BLANK) {}
        TextButton(string text, float fontSize, Vector2 position, Color fg, Color bg = BLANK, Color fgHover = BLANK, Color bgHover = BLANK, Padding padding = {0, 0, 0, 0}, Font font = GetFontDefault());
        void init() override;
        void update() override;
        void draw() override;
        string getType() { return "TextButton"; }
        Color getFgHover() { return fgHover; }
        Color getBgHover() { return bgHover; }
        TextButton& setFgHover(Color fgHover) { this->fgHover = fgHover; return *this; }
        TextButton& setBgHover(Color bgHover) { this->bgHover = bgHover; return *this; }
        TextButton& OnClick(void (*onClick)(TextButton* self)) {
            this->onClick = onClick;
            return *this;
        }
        TextButton& OnHover(void (*onHover)(TextButton* self)) {
            this->onHover = onHover;
            return *this;
        }
        TextButton& OnUnhover(void (*onUnhover)(TextButton* self)) {
            this->onUnhover = onUnhover;
            return *this;
        }
};

#endif