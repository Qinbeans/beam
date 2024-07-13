#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <raylib.h>
#include "../event/object.h"

using std::string;

const int PADDING_NUM = 4;

class Text : public Object {
    protected:
        string text;
        float fontSize;
        Color fg;
        Color bg;
        Padding padding; // top, right, bottom, left
        Vector2 size;
        Font font;
    public:
        Text(): text(""), fontSize(20.0f), fg(RED), bg(BLANK), padding({0, 0, 0, 0}), font(GetFontDefault()) {
            id = node_id++;
        }
        /**
         * @brief Construct a new Text object
         * 
         * @param text The text to be displayed
         * @param fontSize The font size
         * @param fg The foreground color
         * @param bg? The background color
         * @param padding? The padding for the text
         */
        Text(string text, float fontSize, Vector2 position, Color fg, Color bg = BLANK, Padding padding = {0, 0, 0, 0}, Font font = GetFontDefault());
        string getType() { return "Text"; }
        string getText() { return text; }
        float getFontSize() { return fontSize; }
        Color getFg() { return fg; }
        Color getBg() { return bg; }
        Text& setText(string text) { this->text = text; return *this; }
        Text& setFontSize(int fontSize) { this->fontSize = fontSize; return *this; }
        Text& setFg(Color fg) { this->fg = fg; return *this; }
        Text& setBg(Color bg) { this->bg = bg; return *this; }
        void init() override;
        void update() override;
        void draw() override;
};


#endif // TEXT_H