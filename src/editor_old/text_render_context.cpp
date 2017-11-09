
#include "text_render_context.hpp"

#include <algorithm>
#include <iostream>
#include <vector>



text_render_context::text_render_context(wxDC *_dc, formatting *frmt):
    dc(_dc),
    screen(0, 0),
    position(1, 1),
    max_line_width(0),
    max_line_height(0),
    format(frmt) {
}


void text_render_context::reset() {
    screen.x = 0;
    screen.y = 0;
    position.x = 1;
    position.y = 1;
}


void text_render_context::print(const std::wstring &content) {
    wxSize sz = get_extents(content);
    assert(content.find(L"\n") == std::wstring::npos);
    dc->DrawText(content, screen.x, screen.y);
    max_line_height = std::max(max_line_height, sz.GetHeight());
    screen.x += sz.GetWidth();
}


wxSize text_render_context::get_extents(const std::wstring &content) {
    if (content == L"") {
        wxSize temp = dc->GetTextExtent(L" ");
        temp.SetWidth(0);
        return temp;
    }
    return dc->GetTextExtent(content);
}


void text_render_context::report() {
    std::cout << "Location: ";
    std::cout << position.x << " ";
    std::cout << position.y << " Screen: ";
    std::cout << screen.x << " ";
    std::cout << screen.y << " ";
    std::cout << std::endl;
}


void text_render_context::new_line() {
    position.y++;
    position.x = 0;
    screen.y += max_line_height;
    max_line_height = 0;
    screen.x = 0;
}
