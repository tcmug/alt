
#include "text_caret.hpp"
#include "text_render_context.hpp"
#include "editor.hpp"

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


void text_caret::notify(event *_event) {
    editor_event *event = static_cast<editor_event*>(_event);
    EditView *source = static_cast<EditView*>(event->source);
    switch (event->type) {

        case editor_event::ERASE_STRING:
            if (position.y == event->position.y) {
                if (position.x == 1) {
                    if (position.y > 1) {
                        position.x = source->lines[position.y - 2].get_length() + 1;
                        position.y--;
                    }
                }
                else if (position.x >= event->position.x) {
                    position.x--;
                }
            }
            mark_dirty();
        break;

        case editor_event::INSERT_STRING:
            if (position.y == event->position.y &&
                position.x >= event->position.x) {
                position.x += event->string.length();
                mark_dirty();
            }
        break;

        case editor_event::INSERT_LINE:
            if (event->position.y < position.y) {
                position.y = position.y + 1;
                mark_dirty();
            }
        break;
    }
}
