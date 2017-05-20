
#include "text_caret.hpp"
#include "text_render_context.hpp"
#include "editor.hpp"

#include <algorithm>


text_caret::text_caret(std::size_t _position, wxPoint _screen, wxSize _extents):
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
    return (position < other.position);
}

bool text_caret::operator == (const text_caret& other) const {
    return (position == other.position);
}


void text_caret::report() {
    std::cout << position << std::endl;
}


void text_caret::notify(event *_event) {
    editor_event *event = static_cast<editor_event*>(_event);
    EditView *source = static_cast<EditView*>(event->source);
    switch (event->type) {
        case editor_event::ERASE_STRING:
            if (position >= event->position) {
                if (position > event->string.length())
                    position -= event->string.length();
                else
                    position = 0;
                mark_dirty();
            }
        break;

        case editor_event::INSERT_STRING:
            if (position >= event->position) {
                position += event->string.length();
                mark_dirty();
            }
        break;
    }
}
