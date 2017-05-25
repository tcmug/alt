#ifndef TEXT_RENDER_CONTEXT_HPP
#define TEXT_RENDER_CONTEXT_HPP

#include "wx/wx.h"
#include "state.hpp"
#include "../misc/regtionary.hpp"

typedef regtionary <std::wstring> formatting;

class text_render_context {
    public:

        text_render_context(wxDC *_dc, formatting *format);
        void print(const std::wstring &content);
        wxSize get_extents(const std::wstring &content);

        wxDC *dc;

        wxPoint position;
        wxPoint screen;

        wxPoint viewport_position;
        wxSize viewport_extents;

        int max_line_width;
        int max_line_height;

        formatting *format;

        void report();
        void reset();
};


#endif
