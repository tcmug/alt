#ifndef TEXT_CARET_HPP
#define TEXT_CARET_HPP

#include "wx/wx.h"

#include "text_render_context.hpp"
#include "dirtyable.hpp"

class text_caret:
    public dirtyable {

    public:

        text_caret(wxPoint _position, wxPoint _screen, wxSize _extents);
        void render(text_render_context &tx) const;

        wxPoint screen;
        wxPoint position;
        wxSize extents;

        bool operator < (const text_caret& other) const;
        bool operator == (const text_caret& other) const;

        void report();
        void update();

};

#endif
