#ifndef _EDITOR_ELEMENT_HPP_
#define _EDITOR_ELEMENT_HPP_

#include <FL/Fl.H>

class element {

    private:

        Fl_Color _color;
        Fl_Color _background;

    public:

        element(): _color(0), _background(0) {}
        element(Fl_Color color): _color(color) {}
        void print();

};

#endif
