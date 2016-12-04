
#include "textedit.hpp"

#include <algorithm>

text_render_context::text_render_context(wxDC *_dc):
    dc(_dc),
    screen(0, 0),
    max_line_width(0),
    max_line_height(0),
    left_padding(20),
    position(1, 1) {
}


void text_render_context::print(std::wstring content) {
    wxSize sz = dc->GetTextExtent(content.c_str());
    dc->DrawText(content.c_str(), screen.x, screen.y);
    screen.x += sz.GetWidth();
    position.x += content.length();
}


bool text_line::render_text(text_render_context &tx) const {
    wxSize sz;
    if (content == L"") {
        sz = tx.dc->GetTextExtent(L" ");
    }
    else {
        sz = tx.dc->GetTextExtent(content.c_str());
    }
    tx.max_line_height = sz.GetHeight();
    tx.print(content);
    tx.max_line_height = std::max(tx.max_line_height, sz.GetHeight());
    return false;
}


text_line::text_line(std::wstring str):
    content(str),
    changed(0)
    {
}


void text_line::render(text_render_context &tx) const {

    tx.max_line_height = 0;
    tx.position.x = 1;
    tx.screen.x = tx.left_padding;

    render_text(tx);

    tx.max_line_width = std::max(tx.max_line_width, tx.screen.x);

    tx.position.y++;
    tx.screen.y += tx.max_line_height;
}



int text_line::map_column_to_point(text_render_context &tx, int x) const {

    // Scan the block for the caret position.
    // @TODO: Optimize.
    tx.screen.x = tx.left_padding;

    if (x > content.length()) {
        wxSize sz = tx.dc->GetTextExtent(content.c_str());
        return tx.screen.x + sz.GetWidth();
    } else if (x <= 1) {
        return tx.screen.x;
    } else {
        std::wstring substring = content.substr(0, x - 1);
        wxSize sz = tx.dc->GetTextExtent(substring.c_str());
        return tx.screen.x + sz.GetWidth();
    }
    return 0;
}



int text_line::map_point_to_column(text_render_context &tx, int x) const {

    // Scan the block for the caret position.
    // @TODO: Optimize.
    wxSize sz = tx.dc->GetTextExtent(content.c_str());
    int temp_x = tx.screen.x;
    tx.screen.x = tx.left_padding;
    tx.max_line_height = sz.y;

    if (x > tx.screen.x + sz.GetWidth()) {
        // Right side of the end of the line.
        return content.length() + 1;
    }
    else if (x < tx.screen.x) {
        // Left side of the start of the line.
        return 1;
    }
    // Scan the line for position.
    for (int i = 0; i < content.length(); i++) {
        std::wstring str = content.substr(0, i);
        int prev_x = temp_x;
        wxSize sz = tx.dc->GetTextExtent(str.c_str());
        temp_x = tx.screen.x + sz.GetWidth();
        if (x <= temp_x) {
            return i;
        }
    }
    // Default to length of line.
    return content.length();
}


text_caret::text_caret(wxPoint _position, wxPoint _screen):
    screen(_screen),
    position(_position) {
}


void text_caret::render(text_render_context &tx) const {
    tx.dc->SetBrush(wxBrush(wxColour(0, 0, 0)));
    tx.dc->DrawRectangle(screen.x, screen.y, 2, tx.max_line_height);
}

void text_line::insert(int pos, std::wstring str) {
    content.insert(pos, str);
}



void text_line::erase(int pos) {
    content.erase(pos, 1);
}


std::wstring text_line::cut(int pos) {
    std::wstring data = content.substr(pos);
    content.erase(pos);
    return data;
}

int text_line::get_length() const {
    return content.length();
}
