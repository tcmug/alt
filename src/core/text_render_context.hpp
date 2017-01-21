#ifndef TEXT_RENDER_CONTEXT_HPP
#define TEXT_RENDER_CONTEXT_HPP

#include "wx/wx.h"


class text_render_context {
    public:

        text_render_context(wxDC *_dc);
        void print(std::wstring content);

        wxDC *dc;

        wxPoint position;
        wxPoint screen;

        int max_line_width;
        int max_line_height;
        int left_padding;
        int char_width;

        void report();
};


#endif
