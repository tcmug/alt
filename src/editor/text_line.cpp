
#include "text_line.hpp"
#include "text_caret.hpp"
#include "text_render_context.hpp"

#include <algorithm>


text_line::text_line(std::wstring str):
    content(str),
    extents(0, 0)
    {
}


void text_line::render(text_render_context &tx) const {

    tx.max_line_height = 0;
    tx.position.x = 1;
    tx.screen.x = tx.left_padding;

    tx.max_line_height = extents.GetHeight();
    tx.print(content);
    tx.max_line_width = std::max(tx.max_line_width, tx.screen.x);

    tx.position.y++;
    tx.screen.y += extents.GetHeight();
}



int text_line::map_column_to_point(text_render_context &tx, int x) const {

    // Scan the block for the caret position.
    // @TODO: Optimize.
    tx.screen.x = tx.left_padding;

    wxSize sz = tx.get_extents(content.c_str());
    tx.max_line_height = std::max(tx.max_line_height, sz.GetHeight());

    if (x > content.length()) {
        return tx.screen.x + sz.GetWidth();
    }
    else if (x <= 1) {
        return tx.screen.x;
    }
    else {
        std::wstring substring = content.substr(0, x - 1);
        sz = tx.get_extents(substring.c_str());
        return tx.screen.x + sz.GetWidth();
    }
    return 0;
}



int text_line::map_point_to_column(text_render_context &tx, int x) const {

    // @TODO: Optimize.
    wxSize sz = tx.get_extents(content.c_str());
    int temp_x = tx.screen.x;
    tx.screen.x = tx.left_padding;
    tx.max_line_height = sz.y;

    if (x > tx.screen.x + sz.GetWidth()) {
        // Right side of the end of the line.
        tx.screen.x = tx.screen.x + sz.GetWidth();
        return content.length() + 1;
    }
    else if (x <= tx.screen.x) {
        // Left side of the start of the line.
        return 1;
    }
    // Scan the line for position.
    for (int i = 0; i < content.length(); i++) {
        std::wstring str = content.substr(0, i);
        int prev_x = temp_x;
        wxSize sz = tx.get_extents(str.c_str());
        temp_x = tx.screen.x + sz.GetWidth();
        int width = (temp_x - prev_x) * 0.5;
        if (x <= temp_x - width) {
            tx.screen.x = prev_x;
            return i;
        }
    }

    // Default to length of line.
    tx.screen.x = temp_x;
    return content.length();
}


void text_line::update(text_render_context &tx) {

    if (content == L"") {
        extents = tx.get_extents(L" ");
    }
    else {
        extents = tx.get_extents(content.c_str());
    }

    //extents.SetHeight(extents.GetHeight());

    screen.x = tx.left_padding;
    screen.y = tx.screen.y;

    tx.screen.y += extents.GetHeight();

    mark_clean();
}


void text_line::insert(int pos, std::wstring str) {
    content.insert(pos, str);
    mark_dirty();
}



void text_line::erase(int pos) {
    content.erase(pos, 1);
    mark_dirty();
}


std::wstring text_line::cut(int pos) {
    std::wstring data = content.substr(pos);
    content.erase(pos);
    mark_dirty();
    return data;
}


int text_line::get_length() const {
    return content.length();
}


int text_line::get_line_height() const {
    return extents.GetHeight();
}
