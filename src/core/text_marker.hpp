#ifndef TEXT_MARKER_HPP
#define TEXT_MARKER_HPP

#include "wx/wx.h"

#include "text_render_context.hpp"

class text_marker {

    private:

        wxPoint start;
        wxPoint end;

        wxPoint screen;
        wxSize extents;
        bool dirty;

    public:

        text_marker(wxPoint _start, wxPoint _end);

        void mark_dirty();
        bool is_dirty();

        wxPoint get_start() const { return start; }
        wxPoint get_end() const { return end; }

        void update(wxPoint start, wxPoint end);

        void render(text_render_context &tx) const;

};


#endif
