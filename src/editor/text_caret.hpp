#ifndef TEXT_CARET_HPP
#define TEXT_CARET_HPP

#include "wx/wx.h"

#include "text_render_context.hpp"
#include "dirtyable.hpp"
#include "editor_event.hpp"

class text_caret:
    public dirtyable,
    public observer {

    public:

        text_caret(std::size_t _position, wxPoint _screen, wxSize _extents);
        void render(text_render_context &tx) const;

        std::size_t position;
        wxPoint screen;
        wxSize extents;

        bool operator < (const text_caret& other) const;
        bool operator == (const text_caret& other) const;

        void report();
        void update();

        void notify(event *_event);

};

#endif
