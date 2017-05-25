
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

    dc->SetTextForeground(wxColour(0, 255, 255));

    for (std::size_t i = 0; i < content.length(); i++) {
        wchar_t c = content.c_str()[i];
        std::wstring str(1, c);
        wxSize sz = get_extents(str);
        wxString wxstr = str;
        std::cout << c << std::endl;
        dc->DrawText(wxstr, screen.x, screen.y);
        screen.x += sz.GetWidth();
    }

    // wxSize sz = get_extents(content.c_str());
    // dc->SetTextForeground(wxColour(0, 255, 255));
    //
    // dc->DrawText(str, screen.x, screen.y);
    // screen.x += sz.GetWidth();
    // position.x += content.length();

/*
    const root_state *state, *current_state;

    current_state = static_cast <const root_state*>(sstack->current());

    int s = 0;
    for (int i = 0; i < content.length(); i++) {

        sstack->handle_input(content[i]);
        state = static_cast <const root_state*>(sstack->current());

        if (state != current_state) {

            i += current_state->get_offset();
            std::wstring str = content.substr(s, i - s);
            s = i;

            dc->SetTextForeground(current_state->get_color());
            wxSize sz = get_extents(str.c_str());
            dc->DrawText(str.c_str(), screen.x, screen.y);
            screen.x += sz.GetWidth();
            position.x += str.length();
            current_state = state;
        }
    }

    std::wstring str = content.substr(s);

    dc->SetTextForeground(current_state->get_color());
    wxSize sz = get_extents(str.c_str());
    dc->DrawText(str.c_str(), screen.x, screen.y);
    screen.x += sz.GetWidth();
    position.x += str.length();
    current_state = state;
    */
}


/*

void text_render_context::print(std::wstring content) {

    const root_state *state, *current_state;

    current_state = static_cast <const root_state*>(sstack->current());

    int s = 0;
    for (int i = 0; i < content.length(); i++) {

        sstack->handle_input(content[i]);
        const root_state *state = static_cast <const root_state*>(sstack->current());

        if (state != current_state) {

            std::wstring str = content.substr(s, i - s);
            s = i;
            dc->SetTextForeground(current_state->get_color());
            dc->DrawText(str.c_str(), screen.x, screen.y);
            wxSize sz = get_extents(str.c_str());
            screen.x += sz.GetWidth();
            position.x += str.length();
            current_state = state;
        }
    }

    std::wstring str = content.substr(s);
    wxSize sz = get_extents(str.c_str());
    dc->SetTextForeground(current_state->get_color());
    dc->DrawText(str.c_str(), screen.x, screen.y);
    screen.x += sz.GetWidth();
    position.x += str.length();
    current_state = state;
}*/



wxSize text_render_context::get_extents(const std::wstring &content) {

    int w = 0;
    int h = 0;
    wxSize sz;

    if (content.empty()) {
        sz = dc->GetTextExtent(" ");
    }
    else {
        sz = dc->GetTextExtent(content);
    }


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
