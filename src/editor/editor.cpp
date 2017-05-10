
#include <wx/event.h>
#include <algorithm>

#include "editor.hpp"

using alt::ide;


BEGIN_EVENT_TABLE(EditView, wxPanel)
    EVT_LEFT_DOWN( EditView::OnLeftDown )
    EVT_MOTION( EditView::OnMotion )
    EVT_LEFT_UP( EditView::OnLeftUp )
    EVT_CHAR( EditView::OnChar )
END_EVENT_TABLE()


#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>


EditView::EditView(wxFrame* parent) :
    wxScrolledWindow(parent) {
    SetCursor(wxCursor(wxCURSOR_IBEAM));
    SetBackgroundColour(wxColour(100, 100, 100));
    SetForegroundColour(wxColour(100, 100, 100));


    std::wifstream t("test.cpp");
    std::wstring str;
    while (std::getline(t, str)) {
        str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
        lines.push_back(text_line(str));
    }

    scale = 1;
    buffer = NULL;

    paint = false;
    font_size = 12;

    sstack = new state_stack<wchar_t>(new root_state());

    font = wxFont("0;12;76;90;90;0;Roboto Mono;49");
    // font = wxFont("0;13;76;90;90;0;Glass TTY VT220;49");
    // font_size, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Roboto");

}


void EditView::OnDraw(wxDC &dc) {
    int x = GetScrollPos(wxHORIZONTAL) / scale;
    int y = GetScrollPos(wxVERTICAL) / scale;
    wxSize sz = render(dc);
    SetScrollbars(1, 1, sz.GetWidth(), sz.GetHeight(), x, y);
}


wxSize EditView::render(wxDC &dc) {

    sstack->reset();

    redraw(dc);

    /*
    wxMemoryDC dc;
    dc.SelectObjectAsSource(*buffer);
    _dc.StretchBlit(
        0, 0,
        buffer->GetWidth(), buffer->GetHeight(),
        &dc,
        0, 0,
        buffer->GetWidth() * scale, buffer->GetHeight() * scale
    );*/

    return canvas_size;
}


void EditView::redraw(wxDC &dc) {


    /*
    wxSize client_size = dc.GetSize();
    const float block_size = 128;
    int block_width = ceil(client_size.GetWidth() / block_size) * block_size;
    int block_height = ceil(client_size.GetHeight() / block_size) * block_size;

    if (buffer == NULL) {
        buffer = new wxBitmap(block_width * scale, block_height * scale, 4);
    }

    if (client_size.GetWidth() * scale >= buffer->GetWidth() ||
        client_size.GetHeight() * scale >= buffer->GetHeight()) {
        delete buffer;
        buffer = new wxBitmap(block_width * scale, block_height * scale, 8);
        std::cout << "enlarged to " << buffer->GetWidth() << " " << buffer->GetHeight() << std::endl;
    }
    */

    //wxMemoryDC dc;
    //dc.SelectObject(*buffer);

    dc.Clear();
    dc.SetFont(font);
    dc.SetUserScale(scale, scale);
    text_render_context tx(&dc);

    // Update and render lines.
    for (auto &line : lines) {
        if (!is_dirty() && !line.is_dirty())
            continue;
        line.update(tx);
    }

    // Update and render carets.
    for (auto &caret : carets) {
        if (!is_dirty() && !caret->is_dirty())
            continue;
        caret->screen = lc_to_trc(caret->position).screen;
        caret->update();
    }

    // Update and render markers.
    for (auto marker : markers) {
        if (!is_dirty() && !marker->is_dirty())
            continue;

        text_render_context start = lc_to_trc(marker->get_start());
        text_render_context end = lc_to_trc(marker->get_end());
        wxPoint lower_right(end.screen.x, end.screen.y + end.max_line_height);
        marker->update(
            start.screen,
            lower_right
        );
    }

    tx.reset();

    // Handle rendering.
    tx.viewport_position.y = GetScrollPos(wxVERTICAL) / scale;
    tx.viewport_extents = dc.GetSize();

    for (const auto &line : lines) {
        line.render(tx);
    }

    for (const auto marker : markers) {
        marker->render(tx);
    }

    canvas_size = wxSize(tx.max_line_width, tx.screen.y);

    // Draw gutter.

    tx.dc->SetBrush(*wxTRANSPARENT_BRUSH);
    tx.dc->DrawRoundedRectangle(
        tx.left_padding, 0,
        1, tx.screen.y,
        2
    );

    mark_clean();

}

bool compareCaret(text_caret *a, text_caret *b) {
    return (*a < *b);
}

void EditView::fix_carets() {

    for (auto i = carets.begin(); i != carets.end();) {
        auto &caret = *i;
        if (caret->position.y > lines.size())
            i = carets.erase(i);
        else
            i++;
    }

    // Sort carets in the expected format (top to bottom).
    std::sort(carets.begin(), carets.end(), compareCaret);

    // And get rid of dupes.
    // carets.erase(
    //     unique(carets.begin(), carets.end()),
    //     carets.end()
    // );
}


void EditView::OnLeftDown( wxMouseEvent& event ) {

    if (!event.AltDown()) {
        clear_carets();
        clear_markers();
    }

    wxClientDC dc(this);

    wxPoint screen = event.GetLogicalPosition(dc);

    screen.x /= scale;
    screen.y /= scale;

    screen.x += GetScrollPos(wxHORIZONTAL) / scale;
    screen.y += GetScrollPos(wxVERTICAL) / scale;

    text_render_context tx = pt_to_trc(screen);

    text_caret *caret = new text_caret(tx.position, tx.screen, wxSize(2, std::max(font_size, tx.max_line_height)));
    caret->subscribe(this);
    carets.push_back(caret);

    paint_start = tx.position;
    paint = true;
    fix_carets();

    Refresh();
}


void EditView::OnLeftUp( wxMouseEvent& event ) {
    paint = false;
}

void EditView::OnMotion( wxMouseEvent& event ) {

    if (!paint) {
        return;
    }

    wxClientDC dc(this);

    wxPoint screen = event.GetLogicalPosition(dc);

    screen.x /= scale;
    screen.y /= scale;

    screen.x += GetScrollPos(wxHORIZONTAL) / scale;
    screen.y += GetScrollPos(wxVERTICAL) / scale;

    text_render_context tx = pt_to_trc(screen);

    this->paint_end = tx.position;

    wxPoint paint_end = this->paint_end;
    wxPoint paint_start = this->paint_start;

    if (paint_end.y < paint_start.y) {
        std::swap(paint_start, paint_end);
    }
    else if (paint_end.y == paint_start.y && paint_end.x < paint_start.x) {
        std::swap(paint_start, paint_end);
    }

    wxPoint temp;

    clear_markers();

    if (paint_start.y == paint_end.y) {

        if (paint_start.x != paint_end.x) {
            text_marker *mar = new text_marker(
                paint_start,
                paint_end
            );
            mar->subscribe(this);
            markers.push_back(mar);
        }

    }
    /*
    else {

        markers.push_back(text_marker(
            paint_start,
            wxPoint(100, paint_start.y)
        ));

        for (int row = paint_start.y + 1; row < paint_end.y; row++) {
            markers.push_back(text_marker(
                wxPoint(0, row),
                wxPoint(100, row)
            ));
        }

        markers.push_back(text_marker(
            wxPoint(0, paint_end.y),
            wxPoint(paint_end.x, paint_end.y)
        ));
    }
    */

    // std::cout << paint_start.x << ":" << paint_start.y << " -> ";
    // std::cout << paint_end.x << ":" << paint_end.y;
    // std::cout << std::endl;

    //carets.push_back(text_caret(tx.position, tx.screen, wxSize(2, std::max(font_size, tx.max_line_height))));

    /*
    markers.push_back(text_marker(
        tx.position,
         wxPoint(100, tx.position.y)
    ));
    */

    fix_carets();

    Refresh();
}





text_render_context EditView::lc_to_trc(wxPoint position) {

    wxClientDC dc(this);
    text_render_context tx(&dc);
    dc.SetFont(font);
    tx.position = wxPoint(1, 1);

    for (const auto &line : lines) {
        if (position.y != tx.position.y) {
            tx.position.y++;
            tx.screen.y += line.get_line_height();
            continue;
        }
        tx.position.x = std::min(line.get_length() + 1, position.x);
        tx.screen.x = line.map_column_to_point(tx, position.x);
        break;
    }

    return tx;
}


text_render_context EditView::pt_to_trc(wxPoint pt) {

    wxClientDC dc(this);
    dc.SetFont(font);
    text_render_context tx(&dc);
    tx.position.x = -1;

    //std::cout << pt.x << " " << pt.y << std::endl;

    for (const auto &line : lines) {
        if ((pt.y > (tx.screen.y + font_size))) {
            tx.position.y++;
            tx.max_line_height = line.get_line_height();
            tx.screen.y += line.get_line_height();
            continue;
        }
        tx.position.x = line.map_point_to_column(tx, pt.x);
        break;
    }

    //std::cout << pt.x << " " << pt.y << " " << tx.screen.y << std::endl;

    if (tx.position.x == -1) {
        tx.position.x = (lines.end()-1)->map_point_to_column(tx, 100000);
        tx.screen.y -= font_size;
    }

    tx.position.y = std::min((int)lines.size(), tx.position.y);

    return tx;
}


void EditView::OnChar(wxKeyEvent& event) {

    wxChar uc = event.GetUnicodeKey();
    wxClientDC dc(this);

    if ( uc != WXK_NONE ) {

        switch (uc) {

            case 127:
                // Backspace.
                erase();
            break;

            case 9: {
                // Tab.
                insert(L"Sameline\n\nEmpty line before this\nAt the end");
            }
            break;

            case 13: {
                // Return.
                insert_new_line();
                // std::wstring str(L"\n");
                // insert(str);
            }
            break;

            default:
                if (event.ControlDown()) {
                    switch (uc) {
                        case 45:
                            //scale = std::max(scale - 0.1, 0.1);
                            font_size = std::max(font_size - 1, 3);
                            font = wxFont(font_size, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
                            mark_dirty();
                        break;
                        case 43:
                            //scale = scale + 0.1;
                            font_size = font_size + 1;
                            font = wxFont(font_size, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
                            mark_dirty();
                        break;
                    }
                }
                else if ( uc >= 32 ) {
                    std::wstring str(1, uc);
                    insert(str);
                }
            break;
        }

    }
    else {

        switch ( event.GetKeyCode() ) {
            case WXK_BACK:
                std::cout << "back: " << event.GetKeyCode() << std::endl;
            break;
            case WXK_DELETE:
                std::cout << "del: " <<  event.GetKeyCode() << std::endl;
            break;

            case WXK_LEFT:
                for (auto &caret : carets) {
                    if (caret->position.x > 1) {
                        caret->position.x--;
                    }
                    else {
                        if (caret->position.y > 1) {
                            caret->position.y--;
                            caret->position.x = lines[caret->position.y - 1].get_length() + 1;
                        }
                        else {
                            caret->position.x = 1;
                        }
                    }
                    caret->screen = lc_to_trc(caret->position).screen;
                }
            break;

            case WXK_RIGHT:
                for (auto &caret : carets) {
                    if (caret->position.x < lines[caret->position.y - 1].get_length() + 1) {
                        caret->position.x++;
                    }
                    else {
                        if (caret->position.y < lines.size()) {
                            caret->position.y++;
                            caret->position.x = 1;
                        }
                        else {
                            caret->position.x = lines[caret->position.y - 1].get_length() + 1;
                        }
                    }
                    caret->screen = lc_to_trc(caret->position).screen;
                }
            break;

            case WXK_UP:
                for (auto &caret : carets) {
                    if (caret->position.y > 1) {
                        caret->position.y--;
                        if (caret->position.x > lines[caret->position.y - 1].get_length() + 1) {
                            caret->position.x = lines[caret->position.y - 1].get_length() + 1;
                        }
                    }
                    caret->screen = lc_to_trc(caret->position).screen;
                }
            break;

            case WXK_DOWN:
                for (auto &caret : carets) {
                    if (caret->position.y < lines.size()) {
                        caret->position.y++;
                        if (caret->position.x > lines[caret->position.y - 1].get_length() + 1) {
                            caret->position.x = lines[caret->position.y - 1].get_length() + 1;
                        }
                    }
                    caret->screen = lc_to_trc(caret->position).screen;
                }
            break;

        }

    }

    if (carets.size() == 1) {
        wxPoint point = (*carets.begin())->screen;
        int x = GetScrollPos(wxHORIZONTAL);
        int y = GetScrollPos(wxVERTICAL);
        int width, height;
        GetClientSize(&width, &height);
        if (point.y < y ||
            point.y + font_size > y + height) {
            y = std::max(0, point.y - (height / 2));
            SetScrollbars(1, 1, canvas_size.x, canvas_size.y, x, y);
        }
    }

    for (auto marker : markers) {
        marker->mark_dirty();
    }

    fix_carets();
    Refresh();


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


/*
 a b c d e f
|a b c d|e f
->
 a b c d e f|a b c|e f
*/
void EditView::erase() {

    for (auto icaret = carets.rbegin(); icaret != carets.rend(); icaret++) {
        auto *caret = *icaret;

        if (caret->position.x > 1) {
            if (caret->position.x <= lines[caret->position.y - 1].get_length() + 1) {
                lines[caret->position.y - 1].erase(caret->position.x - 2);
            }
            editor_event ev(editor_event::ERASE_STRING, caret->position);
            notify(&ev);
        }
        else if (caret->position.y > 1) {
            wxPoint pos = caret->position;
            editor_event ev(editor_event::ERASE_STRING, caret->position);
            notify(&ev);
            lines[pos.y - 2].insert(
                lines[pos.y - 2].get_length(),
                lines[pos.y - 1].cut(0)
            );
            lines.erase(lines.begin() + (pos.y - 1));
        }
    }
}


void EditView::insert(std::wstring str) {
/*
    for (auto marker = markers.rbegin(); marker != markers.rend(); marker++) {
        lines[marker->start.y - 1].erase(marker->start.x - 1, marker->end.x - 1);
    }

    markers.clear();
*/

    std::vector <std::wstring> str_lines = split_string<std::wstring>(str, L"\n");
    auto line = *str_lines.begin();
    str_lines.erase(str_lines.begin());
    insert_str(line);

    for (auto &line : str_lines) {
        insert_new_line();
        insert_str(line);
    }
}



void EditView::insert_new_line() {
    for (auto i = carets.begin(); i != carets.end(); i++) {

        auto &caret = (*i);

        editor_event ev(editor_event::INSERT_LINE, caret->position);
        notify(&ev);

        std::wstring leftover = lines[caret->position.y - 1].cut(caret->position.x - 1);
        lines.insert(lines.begin() + caret->position.y, leftover);

        caret->position.y++;
        caret->position.x = 1;
        caret->mark_dirty();
    }
}


void EditView::insert_str(std::wstring str) {
    for (auto i = carets.begin(); i != carets.end(); i++) {
        lines[(*i)->position.y - 1].insert((*i)->position.x - 1, str);
        editor_event ev(editor_event::INSERT_STRING, (*i)->position);
        ev.string = str;
        notify(&ev);
    }
}


void EditView::clear_markers() {
    for (auto mark : markers) {
        delete mark;
    }
    markers.clear();
}


void EditView::clear_carets() {
    for (auto caret : carets) {
        delete caret;
    }
    carets.clear();
}
