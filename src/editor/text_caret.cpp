
#include "text_caret.hpp"
#include "text_render_context.hpp"

#include <algorithm>


text_caret::text_caret(wxPoint _position, wxPoint _screen, wxSize _extents):
    screen(_screen),
    position(_position),
    extents(_extents) {
}


void text_caret::render(text_render_context &tx) const {
    tx.dc->SetBrush(wxBrush(wxColour(0, 0, 0)));
    tx.dc->DrawRectangle(
        screen.x, screen.y,
        extents.GetWidth(), extents.GetHeight()
    );
}


void text_caret::update() {
    mark_clean();
}

bool text_caret::operator < (const text_caret& other) const {
    if (position.y == other.position.y)
        return (position.x < other.position.x);
    return (position.y < other.position.y);
}

bool text_caret::operator == (const text_caret& other) const {
    return (position.y == other.position.y && position.x == other.position.x);
}


void text_caret::report() {
    std::cout << position.x << " " << position.y << std::endl;
}
