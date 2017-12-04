
#include "Editor.h"
#include "DrawContext.h"

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Scroll.H>
#include <math.h>

#include <memory.h>
#include <map>
#include <string>
#include <iostream>



int Editor::handle(int e) {
    int ret = Fl_Widget::handle(e);

    switch (e) {

        case FL_PUSH: {
            Point pt(Fl::event_x(), Fl::event_y());
            DrawContext ctx = coordinateToContext(pt);
            pt._x = ctx._x;
            pt._y = ctx._y - ctx._charHeight;
            pt._x -= x();
            pt._y -= y();
            //std::cout << "coord:\t" << ctx._x << " => " << ctx._stopRow << " " << ctx._stopColumn << std::endl;

            pt.set(ctx._stopColumn, ctx._stopRow);
            ctx = positionToContext(pt);
            //std::cout << "\t=> " << ctx._stopX << " " << ctx._stopY << std::endl;

            if (!(Fl::event_state() & FL_META)) {
                carets.clear();
            }
            carets.push_back(Caret(0, Point(ctx._x, ctx._y + fl_descent() - fl_height()), Point(2, ctx._charHeight)));

            redraw();
        }
        break;

        case FL_FOCUS:
        case FL_UNFOCUS:
            ret = 1;                // enables receiving keyboard events
            break;

        case FL_KEYDOWN:            // keyboard key pushed
            if (Fl::event_state() & FL_META) {
                switch (Fl::event_text()[0]) {
                    case '-':
                        if (_fontSize > 5) {
                            _fontSize--;
                        }
                        redraw();
                    break;
                    case '+':
                        if (_fontSize < 100) {
                            _fontSize++;
                        }
                        redraw();
                    break;
                }
            }
            else {
                // if (Fl::event_key() == FL_Down) {
                //     position(x(), y()-5);
                // }
                // else if (Fl::event_key() == FL_Up) {
                //     position(x(), y()+5);
                // }
                // else if (Fl::event_key() == FL_Left) {
                //     position(x()-5, y());
                // }
                // else if (Fl::event_key() == FL_Right) {
                //     position(x()+5, y());
                // }
                redraw();
                insert(Fl::event_text(), Fl::event_length());
            }
            break;

        case FL_SHORTCUT:           // incase widget that isn't ours has focus
        case FL_KEYUP:              // keyboard key released
           // printf("%s\n"s Fl::event_text());
            break;


        case FL_ENTER:
            ret = 1; // FL_ENTER: must return(1) to receive FL_MOVE
            break;

        case FL_MOVE:
            //printf("x=%d y=%d\n", (int)Fl::event_x(), (int)Fl::event_y());
            ret = 1;
            break;
    }

    return (ret);
}


void Editor::insert(const char *str, size_t length) {
    //printf("%s\n", str);
    for (auto &caret : carets) {
        EditorEvent event(EditorEvent::INSERT_STRING, caret._position);
        notify(&event);
    }
}



void Editor::draw() {

    fl_color(color());
    fl_rectf(x(), y(), w(), h());

    fl_font(_font, _fontSize);
    fl_color(7);

    Formatting::Result res = _format->scan(_content);

    float sx = x();
    float sy = (float)y() - fl_descent() + fl_height();

   // printf("%i\n", y());

    DrawContext ctx(x(), y(), sx, sy, &res, &_lineStates);
    ctx._leftMargin = sx;

    while (res.next()) {
        res.getCurrentNode()->getValue()->print(&ctx);
    }

    for (auto &caret : carets) {
       caret.render(ctx);
    }
}


Point Editor::positionToCoordinate(const Point &position) {

    Formatting::Result res = _format->scan(_content);

    float sx = x();
    float sy = (float)y() - fl_descent() + fl_height();

    DrawContext ctx(x(), y(), sx, sy, &res, &_lineStates);

    ctx._render = false;
    ctx._leftMargin = sx;
    ctx._stopCondition = DrawContext::POSITION;

    while (res.next()) {
        res.getCurrentNode()->getValue()->print(&ctx);
    }

}



Point Editor::coordinateToPosition(const Point &coordinate) {

    Formatting::Result res = _format->scan(_content);

    float sx = x();
    float sy = (float)y() - fl_descent() + fl_height();

    DrawContext ctx(x(), y(), sx, sy, &res, &_lineStates);

    ctx._render        = false;
    ctx._leftMargin    = sx;
    ctx._stopCondition = DrawContext::COORDINATE;
    ctx._stopX         = coordinate._x;
    ctx._stopY         = coordinate._y;

    while (res.next()) {
        res.getCurrentNode()->getValue()->print(&ctx);
        if (ctx._stopConditionMet) {
            break;
        }
    }

    if (!ctx._stopConditionMet) {
        // @FIXME: return value should be last line + columns
        return Point(-1, -1);
    }

    return Point(ctx._stopColumn, ctx._stopRow);
}




DrawContext Editor::coordinateToContext(const Point &coordinate) {

    Formatting::Result res = _format->scan(_content);

    float sx = x();
    float sy = (float)y() - fl_descent() + fl_height();

    DrawContext ctx(x(), y(), sx, sy, &res, &_lineStates);

    ctx._render        = false;
    ctx._leftMargin    = sx;
    ctx._stopCondition = DrawContext::COORDINATE;
    ctx._stopX         = coordinate._x;
    ctx._stopY         = coordinate._y;
    ctx._stopExact     = false;

    while (res.next()) {
        res.getCurrentNode()->getValue()->print(&ctx);
        if (ctx._stopConditionMet) {
            break;
        }
    }

    if (!ctx._stopConditionMet) {
        // @FIXME: return value should be last line + columns
        return ctx;
    }

    return ctx;
}




DrawContext Editor::positionToContext(const Point &position) {

    Formatting::Result res = _format->scan(_content);

    float sx = x();
    float sy = (float)y() - fl_descent() + fl_height();

    DrawContext ctx(x(), y(), sx, sy, &res, &_lineStates);

    ctx._render        = false;
    ctx._leftMargin    = sx;
    ctx._stopCondition = DrawContext::POSITION;
    ctx._stopColumn    = position._x;
    ctx._stopRow       = position._y;
    ctx._stopExact     = false;

    while (res.next()) {
        res.getCurrentNode()->getValue()->print(&ctx);
        if (ctx._stopConditionMet) {
            break;
        }
    }

    if (!ctx._stopConditionMet) {
        return ctx;
    }

    return ctx;
}



Editor::Editor(int X,int Y,int W,int H,const char*L) : Fl_Widget(X,Y,W,H,L) {
    box(FL_FLAT_BOX);
    color(4);

    std::map <std::string, int> fonts;

    int i, k = Fl::set_fonts("-*"), t;
    for (i = 0; i < k; i++) {
        fonts[Fl::get_font_name((Fl_Font)i,&t)] = i;
    }

    auto fi = fonts.find("Roboto Mono");

    if (fi != fonts.end()) {
        _font = fi->second;
    }
    else {
        _font = FL_COURIER;
    }

    _fontSize = 20;
    _content = (char*)malloc(512);
    strcpy(_content, "Hello->world!!\n日本国 --> x\r\n\r\n-- $this->\nroy->日本国->Hello->кошка->кошка->$there - 200;\n\t<?php\n\t\t\"hello\"\n\t?>\nuh->oh\n\n\t1\t2\t3\n\t10\t20\t30\n\t505\t545\t334\r\n\r\nThis is the last line.\r\nHi, it wasn't the last one afterall\t\t\tIT IS THIS ONE.");

    _format = new Formatting();
    _format->getRoot()->setValue(new Element(0xA0A0A000));

    Element *c2 = new Element(0x00FF0000);
    Element *c3 = new Element(0xFFFF0000);
    Element *c4 = new Element(0xFF00FF00);
    Element *c5 = new Element(0x7030FF00);
    ElementNewLine *eol = new ElementNewLine(0xFF00FF00);
    ElementTab *tab = new ElementTab(0x80808000);

    _format->insert("\\r?\\n", eol);
    _format->insert("\\t", tab);
    _format->insert("->", c2);
    _format->insert("[0-9]+", c5);
    _format->insert("-->", c3);
    _format->insert("[-+*=\\/]", c4);

    // Formatting::Node *tag = _format->insert("\\<!--", &c8, Formatting::ENTER);
    // tag->insert("[\\r\\n]+", &eol);
    // tag->insert("-->", &c8, Formatting::DROP);

    // Formatting::Node *php = _format->insert("\\<\\?php", &c8, Formatting::ENTER);
    // php->insert("[\\r\\n]+", &eol);
    // php->insert("\\$[a-zA-Z0-9]+", &c2);
    // php->insert("\\?>", &c8, Formatting::DROP);
    // php->insert("\\\"[^\\\"]*\\\"", &c6);
    // php->insert("\\'[^\\']*\\'", &c7);

}
