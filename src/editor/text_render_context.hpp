#ifndef TEXT_RENDER_CONTEXT_HPP
#define TEXT_RENDER_CONTEXT_HPP

#include "wx/wx.h"
#include "state.hpp"

class text_render_context {
    public:

        text_render_context(wxDC *_dc);
        void print(std::wstring content);
        wxSize get_extents(std::wstring content);

        wxDC *dc;

        wxPoint position;
        wxPoint screen;

        int max_line_width;
        int max_line_height;

        int left_padding;

        int offset_y;
        int lower_y;

        state_stack <wchar_t> *sstack;

        void report();
        void reset();
};


#endif
