
#include <wx/event.h>
#include <algorithm>

#include "editor.hpp"

using alt::ide;


BEGIN_EVENT_TABLE(EditView, wxPanel)
    EVT_LEFT_DOWN( EditView::OnLeftClick )
    EVT_CHAR( EditView::OnChar )
END_EVENT_TABLE()


EditView::EditView(wxFrame* parent) :
    wxScrolledWindow(parent) {
    SetCursor(wxCursor(wxCURSOR_IBEAM));
    SetBackgroundColour(*wxWHITE);

    lines.push_back(text_line(L"Lorem ipsum."));
    lines.push_back(text_line(L"Sit dolor."));
    lines.push_back(text_line(L"Amet."));
    lines.push_back(text_line(L""));
    lines.push_back(text_line(L"Hello world."));

    markers.push_back(text_marker(
        wxPoint(2, 1),
        wxPoint(3, 1)
    ));


    font_size = 12;
    font = wxFont(font_size, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

    //markers.push_back(text_marker(wxPoint(0,0), wxPoint(10,10)));
}


void EditView::OnDraw(wxDC &dc) {
    wxSize sz = render(dc);
    int x = GetScrollPos(wxHORIZONTAL);
    int y = GetScrollPos(wxVERTICAL);
    SetScrollbars(1, 1, sz.GetWidth(), sz.GetHeight(), x, y);
}


wxSize EditView::render(wxDC &dc) {

    // wxFONTFAMILY_TELETYPE
    dc.SetFont(font);

    text_render_context tx(&dc);
    for (const auto &line : lines) {
        line.render(tx);
    }

    canvas_size = wxSize(tx.max_line_width, tx.screen.y);

    for (const auto &caret : carets) {
        caret.render(tx);
    }

    for (auto &marker : markers) {
        if (!marker.is_dirty())
            continue;

        text_render_context start = lc_to_trc(marker.get_start());
        text_render_context end = lc_to_trc(marker.get_end());
        wxPoint lower_right(end.screen.x, end.screen.y + end.max_line_height);
        marker.update(
            start.screen,
            lower_right
        );
    }


    for (const auto &marker : markers) {
        marker.render(tx);
    }

    return canvas_size;

}



void EditView::OnLeftClick( wxMouseEvent& event ) {
    // event.Skip();

    if (!event.AltDown()) {
        carets.clear();
        markers.clear();
    }

    wxClientDC dc(this);

    wxPoint screen = event.GetLogicalPosition(dc);

    screen.x += GetScrollPos(wxHORIZONTAL);
    screen.y += GetScrollPos(wxVERTICAL);

    text_render_context tx = pt_to_trc(screen);

    tx.report();

    carets.push_back(text_caret(tx.position, tx.screen, wxSize(2, 12)));
    std::sort(carets.begin(), carets.end());

    markers.push_back(text_marker(
        wxPoint(0, tx.position.y),
        wxPoint(10000, tx.position.y)
    ));

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
            tx.screen.y += font_size;
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
            tx.screen.y += font_size;
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
                    caret.screen = lc_to_trc(caret.position).screen;
                }
            break;

            case 9: {
                // Tab.
                insert(L"\t");
            }
            break;

            case 13: {
                // Return.
                std::wstring str(1, uc);
                insert(str);
            }
            break;

            default:
                // Characters.
                if ( uc >= 32 ) {
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

    Refresh();
}


template <typename T>
std::vector <T> split_string(T string, T delim) {
    std::vector <T> result;
    size_t from = 0, to = 0;
    while (T::npos != (to = string.find(delim, from))) {
        result.push_back(string.substr(from, to - from));
        from = to + delim.length();
    }
    result.push_back(string.substr(from, to));
    return result;
}


void EditView::insert(std::wstring str) {
    std::vector <T> lines = split_string<std::wstring>(str, L"\n");
    for (auto &caret : carets) {
        lines[caret.position.y - 1].insert(caret.position.x - 1, str);
        caret.position.x += str.length();
        caret.screen = lc_to_trc(caret.position).screen;
    }
}
