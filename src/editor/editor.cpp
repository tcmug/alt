
#include <wx/event.h>
#include <algorithm>

#include "editor.hpp"

using alt::ide;


BEGIN_EVENT_TABLE(EditView, wxPanel)
    EVT_LEFT_DOWN( EditView::OnLeftDown )
    EVT_LEFT_UP( EditView::OnLeftUp )
    EVT_CHAR( EditView::OnChar )
END_EVENT_TABLE()


EditView::EditView(wxFrame* parent) :
    wxScrolledWindow(parent) {
    SetCursor(wxCursor(wxCURSOR_IBEAM));
    SetBackgroundColour(*wxWHITE);
    SetForegroundColour(*wxWHITE);

    lines.push_back(text_line(L"Lorem ipsum."));
    lines.push_back(text_line(L"Sit dolor."));
    lines.push_back(text_line(L"Amet."));
    lines.push_back(text_line(L""));
    lines.push_back(text_line(L"Hello world."));

    markers.push_back(text_marker(
        wxPoint(2, 1),
        wxPoint(3, 1)
    ));

    scale = 2;//GetContentScaleFactor();

    buffer = NULL;
    font_size = 10;
    font = wxFont(font_size, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

}


void EditView::OnDraw(wxDC &dc) {
    int x = GetScrollPos(wxHORIZONTAL);
    int y = GetScrollPos(wxVERTICAL);
    wxSize sz = render(dc);
    SetScrollbars(1, 1, sz.GetWidth(), sz.GetHeight(), x, y);
}


wxSize EditView::render(wxDC &dc) {

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

    wxSize client_size = dc.GetSize();
    /*

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
    // dc.SetUserScale(scale, scale);
    text_render_context tx(&dc);

    tx.offset_y = GetScrollPos(wxVERTICAL);
    tx.lower_y = tx.offset_y + client_size.GetHeight();

    // Update and render lines.
    for (auto &line : lines) {
        if (!is_dirty() && !line.is_dirty())
            continue;
        line.update(tx);
    }

    // Update and render carets.
    for (auto &caret : carets) {
        if (!is_dirty() && !caret.is_dirty())
            continue;
        caret.screen = lc_to_trc(caret.position).screen;
        caret.update();
    }

    // Update and render markers.
    for (auto &marker : markers) {
        if (!is_dirty() && !marker.is_dirty())
            continue;

        text_render_context start = lc_to_trc(marker.get_start());
        text_render_context end = lc_to_trc(marker.get_end());
        wxPoint lower_right(end.screen.x, end.screen.y + end.max_line_height);
        marker.update(
            start.screen,
            lower_right
        );
    }

    tx.reset();

    // Handle rendering.

    for (const auto &line : lines) {
        line.render(tx);
        // if (tx.screen.y > tx.lower_y)
        //     break;
    }

    for (const auto &caret : carets) {
        caret.render(tx);
    }

    for (const auto &marker : markers) {
        marker.render(tx);
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


void EditView::fix_carets() {

    for (std::vector <text_caret>::iterator i = carets.begin(); i != carets.end();) {
        auto &caret = *i;
        if (caret.position.y > lines.size())
            i = carets.erase(i);
        else
            i++;
    }

    // Sort carets in the expected format (top to bottom).
    std::sort(carets.begin(), carets.end());

    // And get rid of dupes.
    carets.erase(
        unique(carets.begin(), carets.end()),
        carets.end()
    );
}


void EditView::OnLeftDown( wxMouseEvent& event ) {

    if (!event.AltDown()) {
        carets.clear();
        markers.clear();
    }

    wxClientDC dc(this);

    wxPoint screen = event.GetLogicalPosition(dc);

    screen.x += GetScrollPos(wxHORIZONTAL);
    screen.y += GetScrollPos(wxVERTICAL);

    text_render_context tx = pt_to_trc(screen);

    carets.push_back(text_caret(tx.position, tx.screen, wxSize(2, std::max(font_size, tx.max_line_height))));

    fix_carets();

    Refresh();
}


void EditView::OnLeftUp( wxMouseEvent& event ) {

    wxClientDC dc(this);

    wxPoint screen = event.GetLogicalPosition(dc);

    screen.x += GetScrollPos(wxHORIZONTAL);
    screen.y += GetScrollPos(wxVERTICAL);

    text_render_context tx = pt_to_trc(screen);

    //carets.push_back(text_caret(tx.position, tx.screen, wxSize(2, std::max(font_size, tx.max_line_height))));

    markers.push_back(text_marker(
        tx.position,
        wxPoint(100, tx.position.y)
    ));

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

    for (const auto &line : lines) {
        if ((pt.y < tx.screen.y) || (pt.y > tx.screen.y + font_size)) {
            tx.position.y++;
            tx.max_line_height = line.get_line_height();
            tx.screen.y += line.get_line_height();
            continue;
        }
        tx.position.x = line.map_point_to_column(tx, pt.x);
        break;
    }

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
                            font_size = std::max(font_size - 1, 3);
                            //scale = std::max(scale - 1, 3);
                            font = wxFont(font_size, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
                            mark_dirty();
                        break;
                        case 43:
                            font_size++;
                            font = wxFont(font_size, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
                            mark_dirty();
                        break;
                    }
                }
                else if ( uc >= 32 ) {
                    std::wstring str(1, uc);
                    insert_str(str);
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
                    if (caret.position.x > 1) {
                        caret.position.x--;
                    }
                    else {
                        if (caret.position.y > 1) {
                            caret.position.y--;
                            caret.position.x = lines[caret.position.y - 1].get_length() + 1;
                        }
                        else {
                            caret.position.x = 1;
                        }
                    }
                    caret.screen = lc_to_trc(caret.position).screen;
                }
            break;

            case WXK_RIGHT:
                for (auto &caret : carets) {
                    if (caret.position.x < lines[caret.position.y - 1].get_length() + 1) {
                        caret.position.x++;
                    }
                    else {
                        if (caret.position.y < lines.size()) {
                            caret.position.y++;
                            caret.position.x = 1;
                        }
                        else {
                            caret.position.x = lines[caret.position.y - 1].get_length() + 1;
                        }
                    }
                    caret.screen = lc_to_trc(caret.position).screen;
                }
            break;

            case WXK_UP:
                for (auto &caret : carets) {
                    if (caret.position.y > 1) {
                        caret.position.y--;
                        if (caret.position.x > lines[caret.position.y - 1].get_length() + 1) {
                            caret.position.x = lines[caret.position.y - 1].get_length() + 1;
                        }
                    }
                    caret.screen = lc_to_trc(caret.position).screen;
                }
            break;

            case WXK_DOWN:
                for (auto &caret : carets) {
                    if (caret.position.y < lines.size()) {
                        caret.position.y++;
                        if (caret.position.x > lines[caret.position.y - 1].get_length() + 1) {
                            caret.position.x = lines[caret.position.y - 1].get_length() + 1;
                        }
                    }
                    caret.screen = lc_to_trc(caret.position).screen;
                }
            break;

        }

    }


    if (carets.size() == 1) {
        wxPoint point = carets[0].screen;
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

    for (auto &marker : markers) {
        marker.mark_dirty();
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


void EditView::erase() {
    for (auto &caret : carets) {
        if (caret.position.x > 1) {
            if (caret.position.x <= lines[caret.position.y - 1].get_length() + 1) {
                lines[caret.position.y - 1].erase(caret.position.x - 2);
            }
            caret.position.x--;
        }
        else if (caret.position.y > 1) {
            caret.position.x = lines[caret.position.y - 2].get_length();
            lines[caret.position.y - 2].insert(
                caret.position.x,
                lines[caret.position.y - 1].cut(0)
            );
            caret.position.x++;
            lines.erase(lines.begin() + (caret.position.y - 1));
            caret.position.y--;
        }
        caret.mark_dirty();
    }
}


void EditView::insert(std::wstring str) {

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
    /*
    a|b|c       2,3
    ->
    a
    |b|c        1,2
    ->
    a
    |b
    |c
    */
    for (std::vector <text_caret>::iterator i = carets.begin(); i != carets.end(); i++) {
        auto &caret = (*i);
        std::wstring leftover = lines[caret.position.y - 1].cut(caret.position.x - 1);
        lines.insert(lines.begin() + caret.position.y, leftover);

        for (std::vector <text_caret>::iterator j = i + 1; j != carets.end(); j++) {
            if ((*j).position.y == caret.position.y) {
                (*j).position.x -= caret.position.x - 1;
            }
            (*j).position.y++;
        }

        caret.position.y++;
        caret.position.x = 1;
        caret.mark_dirty();
    }
}


void EditView::insert_str(std::wstring str) {
    for (std::vector <text_caret>::iterator i = carets.begin(); i != carets.end(); i++) {
        auto &caret = (*i);
        lines[caret.position.y - 1].insert(caret.position.x - 1, str);

        for (std::vector <text_caret>::iterator j = i + 1; j != carets.end(); j++) {
            if ((*j).position.y == caret.position.y) {
                (*j).position.x += str.length();
            }
        }

        caret.position.x += str.length();
        caret.mark_dirty();
    }
}
