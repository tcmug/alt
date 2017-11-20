
#include "Caret.h"
#include "DrawContext.h"
#include "Editor.h"

#include <algorithm>

Caret::Caret(std::size_t _position, Point _screen, Point _extents):
    screen(_screen),
    position(_position),
    extents(_extents) {
}


void Caret::render(DrawContext &tx) const {
    //tx.dc->SetBrush(wxBrush(wxColour(0, 0, 0)));
    // tx.dc->DrawRectangle(
    //     screen._x, screen._y,
    //     extents._x, extents._y
    // );
}


void Caret::update() {

}

bool Caret::operator < (const Caret& other) const {
    return (position < other.position);
}

bool Caret::operator == (const Caret& other) const {
    return (position == other.position);
}


void Caret::report() {
}


void Caret::notify(Event *event_in) {

    EditorEvent *event = static_cast<EditorEvent*>(event_in);
    Editor *source = static_cast<Editor*>(event->source);

    switch (event->type) {

        case EditorEvent::MOVE_LEFT:
            if (position == event->position) {
                if (position > 0) {
                    position--;
                }
                //mark_dirty();
            }
        break;
/*
        case EditorEvent::MOVE_RIGHT:
            if (position == event->position) {
                if (position < source->content.get_length() - 1) {
                    position++;
                }
                //mark_dirty();
            }
        break;

        case EditorEvent::MOVE_UP:
            if (position == event->position) {
                std::size_t line = source->content.position_to_line(position);
                std::size_t col = source->content.position_to_column(position);
                if (line > 0) {
                    line--;
                    position = source->content.line_to_position(line) + std::min(col, source->content.get_line_length(line));
                    //mark_dirty();
                }
            }
        break;

        case EditorEvent::MOVE_DOWN:
            if (position == event->position) {
                std::size_t line = source->content.position_to_line(position);
                std::size_t col = source->content.position_to_column(position);
                if (line < source->content.number_of_lines() - 1) {
                    line++;
                    position = source->content.line_to_position(line) + std::min(col, source->content.get_line_length(line));
                    //mark_dirty();
                }
            }
        break;

        case EditorEvent::ERASE_STRING:
            if (position >= event->position) {
                if (position > event->string.length())
                    position -= event->string.length();
                else
                    position = 0;
                //mark_dirty();
            }
        break;

        case EditorEvent::INSERT_STRING:
            if (position >= event->position) {
                position += event->string.length();
                //mark_dirty();
            }
        break;
    */
    }
}
