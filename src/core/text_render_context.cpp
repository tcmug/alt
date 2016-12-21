
#include "text_render_context.hpp"

#include <algorithm>

text_render_context::text_render_context(wxDC *_dc):
    dc(_dc),
    screen(0, 0),
    max_line_width(0),
    max_line_height(0),
    left_padding(20),
    position(1, 1) {
}


void text_render_context::print(std::wstring content) {
    wxSize sz = dc->GetTextExtent(content.c_str());
    dc->DrawText(content.c_str(), screen.x, screen.y);
    screen.x += sz.GetWidth();
    position.x += content.length();
}

