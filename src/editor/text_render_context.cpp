
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


template <typename T>
std::vector <T> split_string(const T string, T delim) {
    std::vector <T> result;
    size_t from = 0, to = 0;
    while (T::npos != (to = string.find(delim, from))) {
        result.push_back(string.substr(from, to - from));
        from = to + delim.length();
    }
    result.push_back(string.substr(from, to));
    return result;
}



void text_render_context::print(const std::wstring &content) {
    wxSize sz = get_extents(content);
    dc->DrawText(content, screen.x, screen.y);
    screen.x += sz.GetWidth();
}


wxSize text_render_context::get_extents(const std::wstring &content) {
    if (content == L"") {
        return dc->GetTextExtent(L" ");
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
