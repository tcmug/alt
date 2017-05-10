#ifndef TEXT_RENDER_CONTEXT_HPP
#define TEXT_RENDER_CONTEXT_HPP

#include "wx/wx.h"
#include "state.hpp"

class text_render_context {
    public:

        text_render_context(wxDC *_dc);
        void print(const std::wstring &content);
        wxSize get_extents(const std::wstring &content);

        wxDC *dc;

        wxPoint position;
        wxPoint screen;

        wxPoint viewport_position;
        wxSize viewport_extents;

        int max_line_width;
        int max_line_height;

        int left_padding;

        state_stack <wchar_t> *sstack;

        void report();
        void reset();
};


#endif
