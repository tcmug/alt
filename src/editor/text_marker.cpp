#include "text_marker.hpp"


text_marker::text_marker(wxPoint _start, wxPoint _end):
    start(_start),
    end(_end),
    screen(0, 0),
    extents(0, 0) {
}


void text_marker::update(wxPoint start, wxPoint end) {
    screen = start;
    extents = wxSize(end.x - start.x, end.y - start.y);
    mark_clean();
}


void text_marker::render(text_render_context &tx) const {

    tx.dc->SetBrush(*wxTRANSPARENT_BRUSH);
    tx.dc->DrawRoundedRectangle(
        screen.x - 2, screen.y,
        extents.GetWidth() + 4, extents.GetHeight() + 1,
        2
    );

}

