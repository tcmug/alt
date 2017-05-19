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


void text_marker::notify(event *_event) {
    editor_event *event = static_cast<editor_event*>(_event);
    switch (event->type) {
        // case editor_event::INSERT_STRING:
        //     if (start.y == event->position.y &&
        //         start.x >= event->position.x) {
        //         start.x += event->string.length();
        //         end.x += event->string.length();
        //         mark_dirty();
        //     }
        // break;
        // case editor_event::INSERT_LINE:
        //     if (event->position.y < start.y) {
        //         start.y = start.y + 1;
        //         end.y = end.y + 1;
        //         mark_dirty();
        //         std::cout << "NEWLINE" << std::endl;
        //     }
        // break;
    }
}

