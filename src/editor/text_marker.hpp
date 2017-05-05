#ifndef TEXT_MARKER_HPP
#define TEXT_MARKER_HPP

#include "wx/wx.h"
#include "dirtyable.hpp"
#include "../misc/observer.hpp"

#include "editor_event.hpp"
#include "text_render_context.hpp"

class text_marker:
    public dirtyable,
    public observer {

    private:

        friend class EditView;

        wxPoint start;
        wxPoint end;

        wxPoint screen;
        wxSize extents;

    public:

        text_marker(wxPoint _start, wxPoint _end);

        wxPoint get_start() const { return start; }
        wxPoint get_end() const { return end; }

        void update(wxPoint start, wxPoint end);

        void render(text_render_context &tx) const;

        void notify(event *event);

};


#endif
