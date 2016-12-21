
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

    lines.push_back(text_line(L"Lorem ipsum."));
    lines.push_back(text_line(L"Sit dolor."));
    lines.push_back(text_line(L"Amet."));

    font_size = 12;
    font = wxFont(font_size, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
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

    return canvas_size;

}



void EditView::OnLeftClick( wxMouseEvent& event ) {
    // event.Skip();

    if (!event.AltDown()) {
        carets.clear();
    }

    wxClientDC dc(this);

    wxPoint screen = event.GetLogicalPosition(dc);

    screen.x += GetScrollPos(wxHORIZONTAL);
    screen.y += GetScrollPos(wxVERTICAL);

    wxPoint position = screen_to_lc(dc, screen);

    screen = lc_to_screen(dc, position);
    carets.push_back(text_caret(position, screen, wxSize(2, 10)));

    std::sort(carets.begin(), carets.end());

    Refresh();

}



wxPoint EditView::screen_to_lc(wxDC &dc, wxPoint &pt) const {

    dc.SetFont(font);
    wxPoint lc(1, 1);
    text_render_context tx(&dc);

    for (const auto &line : lines) {
        if ((pt.y < tx.screen.x) || (pt.y > tx.screen.x + font_size)) {
            lc.y++;
            tx.screen.x += font_size;
            continue;
        }
        lc.x = line.map_point_to_column(tx, pt.x);
        pt.y = tx.screen.x;
        break;
    }
    lc.y = std::min((int)lines.size(), lc.y);
    return lc;
}




wxPoint EditView::lc_to_screen(wxDC &dc, wxPoint &position) const {

    dc.SetFont(font);

    wxPoint lc(1, 1);
    text_render_context tx(&dc);

    for (const auto &line : lines) {
        if (position.y != lc.y) {
            lc.y++;
            tx.screen.y += font_size;
            continue;
        }
        tx.screen.x = line.map_column_to_point(tx, position.x);
        break;
    }
    return tx.screen;
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
                    caret.screen = lc_to_screen(dc, caret.position);
                }
            break;

            case 9: {
                // Tab.
                insert(L"\t");
            }
            break;

            case 13:
                // Return.
                for (auto &caret : carets) {
                    std::wstring remainder = lines[caret.position.y - 1].cut(caret.position.x - 1);
                    lines.insert(lines.begin()+caret.position.y, text_line(remainder));
                    caret.position.x = 1;
                    caret.position.y++;
                    caret.screen = lc_to_screen(dc, caret.position);
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
                    caret.screen = lc_to_screen(dc, caret.position);
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
                    caret.screen = lc_to_screen(dc, caret.position);
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
                    caret.screen = lc_to_screen(dc, caret.position);
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
                    caret.screen = lc_to_screen(dc, caret.position);
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

    Refresh();
}



void EditView::insert(std::wstring str) {
    wxClientDC dc(this);
    dc.SetFont(font);
    wxPoint lc(1, 1);
    text_render_context tx(&dc);
    for (auto &caret : carets) {
        lines[caret.position.y - 1].insert(caret.position.x - 1, str);
        caret.position.x += str.length();
        caret.screen = lc_to_screen(dc, caret.position);
    }
}
