
#include "text_render_context.hpp"

#include <algorithm>
#include <iostream>

text_render_context::text_render_context(wxDC *_dc):
    dc(_dc),
    max_line_width(0),
    max_line_height(0),
    left_padding(20),
    line_padding(3),
    screen(0, 0),
    position(1, 1),
    char_width(0) {
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



void text_render_context::print(std::wstring content) {
    for (int i = 0; i < content.length(); i++) {
        if (i % 2) {
            dc->SetTextForeground(*wxBLACK);
        } else {
            dc->SetTextForeground(*wxRED);
        }
        std::wstring part(1, content[i]);
        wxSize sz = get_extents(part.c_str());
        dc->DrawText(part.c_str(), screen.x, screen.y);
        screen.x += sz.GetWidth();
        position.x += content.length();
    }
}

wxSize text_render_context::get_extents(std::wstring content) {
    int w = 0;
    int h = 0;
    for (int i = 0; i < content.length(); i++) {
        std::wstring part(1, content[i]);
        wxSize sz = dc->GetTextExtent(part.c_str());
        w += sz.GetWidth();
        if (content[i] == ' ') {
            w += sz.GetWidth();
        }
        h = std::max(sz.GetHeight(), h);
    }
    wxSize sz;
    sz.SetWidth(w);
    sz.SetHeight(h);
    return sz;
}

void text_render_context::report() {
    std::cout << "Location: ";
    std::cout << position.x << " ";
    std::cout << position.y << " Screen: ";
    std::cout << screen.x << " ";
    std::cout << screen.y << " ";
    std::cout << std::endl;
}
