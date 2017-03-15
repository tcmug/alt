
#include "text_render_context.hpp"

#include <algorithm>
#include <iostream>



void root_state::handle_input(state_stack_type *s, wchar_t c) {
    if (c == L'<') {
        s->push(new tag());
    }
}


void tag::handle_input(state_stack_type *s, wchar_t c) {
    if (c == L'"') {
        s->push(new quote());
    }
    else if (previous == L'>') {
        delete s->pop();
    }
    previous = c;
}



void quote::handle_input(state_stack_type *s, wchar_t c) {
    if (previous == L'"' && length > 1) {
        delete s->pop();
    }
    length++;
    previous = c;
}



text_render_context::text_render_context(wxDC *_dc):
    dc(_dc),
    max_line_width(0),
    max_line_height(0),
    left_padding(20),
    screen(0, 0),
    position(1, 1) {

    root_state *st = new root_state();
    sstack = new state_stack<wchar_t>(st);

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



void text_render_context::print(std::wstring content) {


    // for (int i = 0; i < str.length(); i++) {
    //  stack.handle_input(str[i]);
    //  const root_state *state = static_cast <const root_state*> (stack.current());
    //  std::cout << state->get_color() << str[i];
    // }

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

}

wxSize text_render_context::get_extents(std::wstring content) {
    int w = 0;
    int h = 0;
    for (int i = 0; i < content.length(); i++) {
        std::wstring part(1, content[i]);
        wxSize sz = dc->GetTextExtent(part.c_str());
        w += sz.GetWidth();
        // if (content[i] == ' ') {
        //     w += sz.GetWidth();
        // }
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
