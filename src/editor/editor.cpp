
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

    content.read("testfile.txt");

    scale = 1;
    buffer = NULL;

    paint = false;
    font_size = 12;
    font = wxFont("0;12;76;90;90;0;Roboto Mono;49");
    // font = wxFont("0;13;76;90;90;0;Glass TTY VT220;49");
    // font_size, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Roboto");

    format = new formatting();

    color a(wxColour(255,255,255), wxColour(0,0,0));
    color b(wxColour(255,0,255), wxColour(0,0,100));
    color c(wxColour(0,255,0), wxColour(0,100,0));
    color d(wxColour(0,0,255), wxColour(0,0,0));

    formatting::node *tag = format->insert(L"\\<\\/?\\w+", c, formatting::ENTER);
    tag->insert(L"\\\"[^\\\"]*\\\"", a);
    tag->insert(L"\\'[^\\']*\\'", b);
    tag->insert(L"([a-z]+)(?=\\W*\\=)", c);
    tag->insert(L"\\>", b, formatting::DROP);

    auto entry = format->insert(L"\\<\\?(php)?", c, formatting::ENTER);
    entry->insert(L"\\?\\>", d, formatting::DROP);
    entry->insert(L"\\$[A-Za-z0-9_]+", c);

    regtionary <color>::result res = format->scan(content.get_line(5).c_str());

    while (res.next()) {
        std::wstring str(res.at, 0, res.start);
        std::cout << str << "||" << res.snip << "#" << res.current_node->value.background.GetRGB() << "/" << res.current_node->value.foreground.GetRGB() << "#";
    }

    std::cout << std::endl;
}


void EditView::OnDraw(wxDC &dc) {
    int x = GetScrollPos(wxHORIZONTAL) / scale;
    int y = GetScrollPos(wxVERTICAL) / scale;
    wxSize sz = render(dc);
    SetScrollbars(1, 1, sz.GetWidth(), sz.GetHeight(), x, y);
}


wxSize EditView::render(wxDC &dc) {

    redraw(dc);

    return canvas_size;
}


void EditView::update() {

    wxClientDC dc(this);

    dc.SetFont(font);
    dc.SetUserScale(scale, scale);

    text_render_context tx(&dc, format);

    tx.viewport_position.y = GetScrollPos(wxVERTICAL) / scale;
    tx.viewport_extents = dc.GetSize();

    line_states.clear();

    for (std::size_t ln = 0; ln < content.number_of_lines(); ln++) {

        const std::wstring &xstr  = content.get_content();

        const wchar_t *line_start = xstr.c_str() + content[ln].start;
        const wchar_t *line_end   = line_start + content[ln].length;

        regtionary <color>::result res = format->scan(line_start, line_end);

        wxSize extents;
        int x = 0;

       // std::cout << ln << " " << std::endl;

        while (res.next()) {
            std::wstring str(res.at, 0, res.start);
            wxSize e1 = tx.get_extents(str);
            wxSize e2 = tx.get_extents(res.snip);
            x += e1.GetWidth();
            x += e2.GetWidth();
            extents.SetHeight(e1.GetHeight());
            //std::cout << str << "||" << res.snip << "#" << res.current_node->value.background.GetRGB() << "/" << res.current_node->value.foreground.GetRGB() << "#";
        }

        extents.SetWidth(x);

        std::cout << std::endl;
        // std::cout << "ST: " << (unsigned long)res.current_node << std::endl;

        tx.max_line_height = 0;
        tx.position.x = 1;
        tx.screen.x = 0;

        tx.max_line_height = extents.GetHeight();
        tx.max_line_width = std::max(tx.max_line_width, tx.screen.x + extents.GetWidth());
        line_states.push_back(line_state(tx.screen, extents, res.current_node));

        tx.position.y++;
        tx.screen.y += extents.GetHeight();
    }

    canvas_size = wxSize(tx.max_line_width, tx.screen.y);
}


void EditView::redraw(wxDC &dc) {

    dc.Clear();
    dc.SetFont(font);
    dc.SetUserScale(scale, scale);
    dc.SetLogicalOrigin(-20, 0);

    text_render_context tx(&dc, format);

    tx.viewport_position.y = GetScrollPos(wxVERTICAL) / scale;
    tx.viewport_extents = dc.GetSize();

    update();

    const std::wstring &xstr = content.get_content();

    for (std::size_t ln = 0; ln < content.number_of_lines(); ln++) {

        if (line_states[ln].screen.y >= tx.viewport_position.y - line_states[ln].extents.GetHeight()) {

            wxSize extents = line_states[ln].extents;

            const wchar_t *line_start = xstr.c_str() + content[ln].start;
            const wchar_t *line_end   = line_start + content[ln].length;

            regtionary <color>::result res = format->scan(line_start, line_end);

            if (ln > 0) {
                res.current_node = line_states[ln-1].current_node;
            }

            tx.max_line_height = 0;
            tx.position.x = 1;
            tx.screen = line_states[ln].screen;

            tx.max_line_height = extents.GetHeight();
            int i = 0;
            while (res.next()) {
                i++;
                std::wstring str(res.at, 0, res.start);
                std::cout << str << std::endl;
                if (res.current_node) {
                    tx.dc->SetTextBackground(res.current_node->value.background);
                    tx.dc->SetTextForeground(res.current_node->value.foreground);
                }
                if (str.length() > 0) {
                    std::cout << "real ln " << ln << ":" << i << " " << str << std::endl;
                    tx.print(str);
                }
                if (res.snip.length() > 0) {
                    std::cout << "snip ln " << ln << ":" << i << " " << str << std::endl;
                    tx.print(res.snip);
                }
            }
            std::cout << std::endl;
            //std::cout << ln << ": " << i << " " << res.start << std::endl;
            tx.max_line_width = std::max(tx.max_line_width, tx.screen.x);

            tx.position.y++;
            tx.screen.y += extents.GetHeight();
        }

        if (line_states[ln].screen.y > tx.viewport_position.y + tx.viewport_extents.GetHeight()) {
            break;
        }
    }

    for (auto &caret : carets) {

        if (caret->is_dirty()) {
            wxPoint position = wxPoint(content.position_to_column(caret->position), content.position_to_line(caret->position));
            std::wstring str = content.get_line(position.y);
            wxSize sz = dc.GetTextExtent(str.substr(0, position.x));

            caret->screen = wxPoint(sz.GetWidth(), line_states[position.y].screen.y);
            caret->extents = wxSize(2, line_states[position.y].extents.GetHeight());
            caret->mark_clean();
        }
        caret->render(tx);
    }
}





bool compareCaret(text_caret *a, text_caret *b) {
    return (a->position == b->position);
}



void EditView::fix_carets() {

    // for (auto i = carets.begin(); i != carets.end();) {
    //     auto &caret = *i;
    //     if (caret->position.y > lines.size())
    //         i = carets.erase(i);
    //     else
    //         i++;
    // }

    // Sort carets in the expected format (top to bottom).

    std::sort(carets.begin(), carets.end(), compareCaret);

    // And get rid of dupes.
    // for (auto i = carets.begin() + 1; i != carets.end(); i++) {
    //     if ((*i)->position == (*(i-1))->position) {
    //         std::cout << (*i)->position << " " << (*(i-1))->position << std::endl;
    //         //i = carets.erase(i);
    //     }
    // }
    // carets.erase(
    //     unique(carets.begin(), carets.end()),
    //     carets.end()
    // );

    carets.erase(unique(carets.begin(), carets.end(), compareCaret), carets.end());

    // int i = 1;
    // for (auto &caret : carets) {
    //     std::cout << i++ << "\t" << caret->position << std::endl;
    // }
}


void EditView::OnLeftDown( wxMouseEvent& event ) {

    if (!event.AltDown()) {
        clear_carets();
        clear_markers();
    }

    wxClientDC dc(this);
    dc.SetFont(font);
    dc.SetUserScale(scale, scale);
    dc.SetLogicalOrigin(-20, 0);
    wxPoint screen = event.GetLogicalPosition(dc);

    screen.x /= scale;
    screen.y /= scale;

    screen.x += GetScrollPos(wxHORIZONTAL) / scale;
    screen.y += GetScrollPos(wxVERTICAL) / scale;

    for (std::size_t ln = 0; ln < content.number_of_lines(); ln++) {
        if ((screen.y > line_states[ln].screen.y) &&
            (screen.y < line_states[ln].screen.y + line_states[ln].extents.GetHeight())) {

            wxPoint position = line_states[ln].screen;
            std::wstring c = content.get_line(ln);
            position.x = dc.GetTextExtent(c.c_str()).GetWidth();
            int i = 0;
            for (; i < c.length(); i++) {
                wxString str = c.substr(0, i);
                wxSize sz = dc.GetTextExtent(str);
                if (screen.x <= sz.GetWidth()) {
                    position.x = sz.GetWidth();
                    break;
                }
            }

            text_caret *car = new text_caret(content[ln].start + i, position, wxSize(2, line_states[ln].extents.GetHeight()));
            car->subscribe(this);
            carets.push_back(car);

            break;
        }
    }

    paint = true;

    Refresh();
}


void EditView::OnLeftUp( wxMouseEvent& event ) {
    paint = false;
}



void EditView::OnMotion( wxMouseEvent& event ) {

    if (!paint) {
        return;
    }

    // wxClientDC dc(this);
    // wxPoint screen = event.GetLogicalPosition(dc);

    // screen.x /= scale;
    // screen.y /= scale;

    // screen.x += GetScrollPos(wxHORIZONTAL) / scale;
    // screen.y += GetScrollPos(wxVERTICAL) / scale;

    // for (std::size_t ln = 0; ln < content.number_of_lines(); ln++) {
    //     if ((screen.y > line_states[ln].screen.y) &&
    //         (screen.y < line_states[ln].screen.y + line_states[ln].extents.GetHeight())) {
    //         carets.push_back(new text_caret(0, line_states[ln].screen, line_states[ln].extents));
    //         break;
    //     }
    // }

    Refresh();
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
                insert(L"\t");
            }
            break;

            case 13: {
                // Return.
                //insert_new_line();
                insert(L"\n");
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
                /*for (auto &caret : carets) {
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
                */
            break;

            case WXK_RIGHT:
            /*
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
                */
            break;

            case WXK_UP:
            /*
                for (auto &caret : carets) {
                    if (caret->position.y > 1) {
                        caret->position.y--;
                        if (caret->position.x > lines[caret->position.y - 1].get_length() + 1) {
                            caret->position.x = lines[caret->position.y - 1].get_length() + 1;
                        }
                    }
                    caret->screen = lc_to_trc(caret->position).screen;
                }
                */
            break;

            case WXK_DOWN:
            /*
                for (auto &caret : carets) {
                    if (caret->position.y < lines.size()) {
                        caret->position.y++;
                        if (caret->position.x > lines[caret->position.y - 1].get_length() + 1) {
                            caret->position.x = lines[caret->position.y - 1].get_length() + 1;
                        }
                    }
                    caret->screen = lc_to_trc(caret->position).screen;
                }
                */
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
    //for (auto icaret = carets.rbegin(); icaret != carets.rend(); icaret++) {
    for (auto &caret : carets) {
        // auto caret = *icaret;
        if (caret->position <= 0) {
            continue;
        }
        //std::cout << caret->position << std::endl;
        content.erase(caret->position - 1, 1);
        editor_event event(editor_event::ERASE_STRING, caret->position - 1);
        event.string = '?';
        notify(&event);
    }
}


void EditView::insert(std::wstring str) {
    for (auto &caret : carets) {
        content.insert(caret->position, str);
        editor_event event(editor_event::INSERT_STRING, caret->position);
        event.string = str;
        notify(&event);
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
