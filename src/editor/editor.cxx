
#include "editor.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Scroll.H>
#include <math.h>

#include <regex>
#include <map>
#include <string>

int Editor::handle(int e) {
    int ret = Fl_Widget::handle(e);

    switch (e) {

        case FL_FOCUS:
        case FL_UNFOCUS:
            ret = 1;                // enables receiving keyboard events
            break;

        case FL_SHORTCUT:           // incase widget that isn't ours has focus
        case FL_KEYDOWN:            // keyboard key pushed
        case FL_KEYUP:              // keyboard key released
            //printf("%i\n", Fl::event_key());
            break;

        case FL_ENTER:
            ret = 1; // FL_ENTER: must return(1) to receive FL_MOVE
            break;

        case FL_MOVE: // FL_MOVE: mouse movement causes 'user damage' and redraw..
            //damage(FL_DAMAGE_USER1);
            //printf("x=%d y=%d\n", (int)Fl::event_x(), (int)Fl::event_y());
            ret = 1;
            break;
    }

    return (ret);
}


void Editor::draw() {

    fl_color(4);
    //printf("%i %i\n", x(), w());
    fl_rectf(x(),y(),w(),h());

    fl_font(_font, 13);
    fl_color(7);

    const char *str = "$this->roy->日本国->Hello->кошка->кошка->$there - 200; <?php \"hello\" ?> uh->oh";

    auto res = _format->scan(str);
    int dx, dy, ww, hh;
    int xp = x(), yp = y() - fl_descent() + fl_height();

    while (res.next()) {
        res.getCurrentNode()->getValue().print();
        fl_draw(res.getAt(), res.getLength(), xp, yp);
        yp += fl_height();
        xp += fl_width(res.getAt(), res.getLength());
    }

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

    auto fi = fonts.find("VT220");

    if (fi != fonts.end()) {
        _font = fi->second;
    }
    else {
        _font = FL_COURIER;
    }

    _format = new formatting();

    element c1(0);
    element c2(1);
    element c3(2);
    element c4(3);
    element c5(4);
    element c6(5);
    element c7(6);
    element c8(7);

    _format->insert("[0-9]+", c1);
    _format->insert("->", c3);
    _format->insert("[-+*=\\/]", c4);

    formatting::node *tag = _format->insert("\\<!--", c8, formatting::ENTER);
    tag->insert("-->", c8, formatting::DROP);

    formatting::node *php = _format->insert("\\<\\?php", c8, formatting::ENTER);
    php->insert("\\$[a-zA-Z0-9]+", c2);
    php->insert("\\?>", c8, formatting::DROP);
    php->insert("\\\"[^\\\"]*\\\"", c6);
    php->insert("\\'[^\\']*\\'", c7);

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
