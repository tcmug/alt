
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
            Point pos = coordinateToPosition(pt);
            std::cout << pt._x << " " << pt._y << " >>> ";
            std::cout << pos._x << " " << pos._y << std::endl;
        }
        break;

        case FL_FOCUS:
        case FL_UNFOCUS:
            ret = 1;                // enables receiving keyboard events
            break;

        case FL_KEYDOWN:            // keyboard key pushed
            insert(Fl::event_text(), Fl::event_length());
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
    printf("%s\n", str);
    for (auto &caret : carets) {
        EditorEvent event(EditorEvent::INSERT_STRING, caret->position);
        notify(&event);
    }
}



void Editor::draw() {

    fl_color(4);
    fl_rectf(x(), y(), w(), h());

    fl_font(_font, _fontSize);
    fl_color(7);

    Formatting::Result res = _format->scan(_content);

    float sx = x();
    float sy = (float)y() - fl_descent() + fl_height();

    DrawContext ctx(sx, sy, &res, &_lineStates);
    ctx._leftMargin = sx;

    while (res.next()) {
        res.getCurrentNode()->getValue()->print(&ctx);
    }

    const char *s = "Hello日本国Worldкошка\u2424";
    size_t i, col, a = 0;
    for (col=i=0; s[i]; i++) {
        if ((unsigned char)s[i] < 0x80 || (unsigned char)s[i] > 0xBF) {
            fl_draw(&s[a], i-a, ctx._x, ctx._y);
            ctx._x += fl_width(&s[a], i-a);
            a = i;
            col++;
        }
    }


}


Point Editor::positionToCoordinate(const Point &position) {

    Formatting::Result res = _format->scan(_content);

    float sx = x();
    float sy = (float)y() - fl_descent() + fl_height();

    DrawContext ctx(sx, sy, &res, &_lineStates);

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

    DrawContext ctx(sx, sy, &res, &_lineStates);

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

    std::cout << ctx._column << " " << ctx._row << std::endl;

    return Point(ctx._stopColumn, ctx._stopRow);
}


void Editor::Timer_CB(void *userdata) {
    Editor *o = (Editor*)userdata;
    o->redraw();
    Fl::repeat_timeout(0.1, Timer_CB, userdata);
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
    strcpy(_content, "日本国 -- $this->\nroy->日本国->Hello->кошка->кошка->$there - 200;\n\t<?php\n\t\t\"hello\"\n\t?>\nuh->oh\n\n\t1\t2\t3\n\t10\t20\t30\n\t505\t545\t334");

    _format = new Formatting();
    _format->getRoot()->setValue(new Element(0x808080));

    Element *c2 = new Element(0x00FF0000);
    ElementNewLine *eol = new ElementNewLine(0xFF00FF00);
    ElementTab *tab = new ElementTab(0x80808000);

    _format->insert("[\\r\\n]+", eol);
    _format->insert("\\t", tab);
    _format->insert("->", c2);
    // _format->insert("[0-9]+", &c1);
    // _format->insert("->", &c3);
    // _format->insert("[-+*=\\/]", &c4);

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


/*

    wchar_t *src = L"World кошка 日本国";
    char dst[256];

    int rl = fl_utf8fromwc(dst, 256, src, 15);
    printf("%s POS: %i\n", dst, wcsstr(src, L"к") - src);
    printf("Length %i (%i)\n", strlen(dst), rl);

    // std::locale old;
    // std::locale::global(std::locale("en_US.UTF-8"));

    std::regex pattern("(日|本国)+", std::regex_constants::extended);
    bool result = std::regex_match(std::string("本国"), pattern);

    // std::locale::global(old);

    if (result) {
        printf("Unicode!\n");
    }

    //content.read("testfile.txt");
}
*/
