
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
    SetForegroundColour(wxColour(255, 255, 255));

    content.read("testfile.txt");

    scale = 1;
    buffer = NULL;

    paint = false;
    font_size = 12;
    font = wxFont("0;12;76;90;90;0;Roboto Mono;49");
    // font = wxFont("0;13;76;90;90;0;Glass TTY VT220;49");
    // font_size, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Roboto");

    format = new formatting();

    color c1(wxColour(100, 100, 100), wxColour(100, 60, 100));
    color c2(wxColour(255, 100, 100), wxColour(100, 100, 100));
    color c3(wxColour(100, 255, 100), wxColour(100, 60, 100));
    color c4(wxColour(100, 100, 255), wxColour(100, 100, 100));
    color c5(wxColour(255, 100, 255), wxColour(100, 100, 100));
    color c6(wxColour(255, 100, 255), wxColour(100, 100, 100));
    color c7(wxColour(100, 100, 255), wxColour(100, 100, 100));
    color c8(wxColour(255, 40, 40), wxColour(100, 100, 100));

    format->insert(L"[0-9]+", c1);
    format->insert(L"->", c3);
    format->insert(L"[-+*=\\/]", c4);

    formatting::node *tag = format->insert(L"\\<!--", c8, formatting::ENTER);
    tag->insert(L"-->", c8, formatting::DROP);

    formatting::node *php = format->insert(L"\\<\\?php", c8, formatting::ENTER);
    php->insert(L"\\$[a-zA-Z0-9]+", c2);
    php->insert(L"\\?>", c8, formatting::DROP);
    php->insert(L"\\\"[^\\\"]*\\\"", c6);
    php->insert(L"\\'[^\\']*\\'", c7);

    // std::cout << "DUMPING" << std::endl;
    // regtionary <color>::result res = format->scan(content.get_content().c_str());
    // while (res.next()) {
    //     std::wstring str(res.at, 0, res.start);
    //     std::cout << str << "||" << res.snip << "#" << res.current_node->value.background.GetRGB() << "/" << res.current_node->value.foreground.GetRGB() << "#";
    // }

    //std::cout << std::endl;
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

}


void EditView::redraw(wxDC &dc) {

    dc.Clear();
    dc.SetFont(font);
    dc.SetUserScale(scale, scale);
    dc.SetLogicalOrigin(-20, 0);

    text_render_context tx(&dc, format);

    tx.viewport_position.y = GetScrollPos(wxVERTICAL) / scale;
    tx.viewport_extents = dc.GetSize();

    const std::wstring &xstr = content.get_content();

    auto res = format->scan(xstr.c_str(), xstr.c_str() + xstr.length());

    line_states.clear();
    scan_context ctx(&line_states, &res, &tx);

    while (res.next()) {
        res.getCurrentNode()->getValue().print(&ctx);
    }

    for (auto &state : line_states) {
        state.report();
    }

    for (auto &caret : carets) {

        if (caret->is_dirty()) {
            wxPoint position = wxPoint(content.position_to_column(caret->position), content.position_to_line(caret->position));
            std::wstring str = content.get_line(position.y);
            wxSize sz = dc.GetTextExtent(str.substr(0, position.x));

            caret->screen = wxPoint(sz.GetWidth(), line_states[position.y].getScreen().y);
            caret->extents = wxSize(2, line_states[position.y].getExtents().GetHeight());
            caret->mark_clean();
        }
        caret->render(tx);
    }

    canvas_size = wxSize(tx.max_line_width, tx.screen.y);
}





bool compareCaret(text_caret *a, text_caret *b) {
    return (a->position == b->position);
}



void EditView::fix_carets() {
    // Sort carets in the expected format (top to bottom).
    std::sort(carets.begin(), carets.end(), compareCaret);
    // And get rid of dupes.
    carets.erase(unique(carets.begin(), carets.end(), compareCaret), carets.end());
}



wxPoint EditView::fix_to_char(wxClientDC &dc, const wxPoint &point, int ln, int &offset) const {
    wxPoint position = line_states[ln].getScreen();
    std::wstring c = content.get_line(ln);
    position.x = dc.GetTextExtent(c.c_str()).GetWidth();
    for (offset = 0; offset < c.length(); offset++) {
        wxString str = c.substr(0, offset);
        wxSize sz = dc.GetTextExtent(str);
        if (point.x <= sz.GetWidth()) {
            position.x = sz.GetWidth();
            break;
        }
    }
    return position;
}


/*
caret position = position in file (i)
point on screen = point on screen (x, y)
row and column = position in file translated to row and column (r, c) when content is split into lines with a delimiter

Provide these:

 - point on screen to caret position
 - point on screen to row and column

 - caret position to point on screen
 - caret position to row and column

 - row and column to point on screen
 - row and column to caret position

*/
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

        if (ln >= line_states.size()) {
            break;
        }

        if (point_on_line(screen, ln)) {

            int offset = 0;
            wxPoint position = fix_to_char(dc, screen, ln, offset);

            text_caret *car = new text_caret(content[ln].start + offset, position, wxSize(2, line_states[ln].getExtents().GetHeight()));
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


bool EditView::point_on_line(const wxPoint &point, int ln) const {
    return (point.y > line_states[ln].getScreen().y) && (point.y < line_states[ln].getScreen().y + line_states[ln].getExtents().GetHeight());
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

    for (std::size_t ln = 0; ln < content.number_of_lines(); ln++) {
        if (point_on_line(screen, ln)) {
            carets.push_back(new text_caret(content[ln].start, line_states[ln].getScreen(), line_states[ln].getExtents()));
            break;
        }
    }

    fix_carets();
    std::cout << carets.size() << std::endl;

    Refresh();
}



void EditView::OnChar(wxKeyEvent& event) {

    wxChar uc = event.GetUnicodeKey();
    wxClientDC dc(this);

    if ( uc != WXK_NONE ) {

        switch (uc) {

            case 127:
                erase();
            break;

            case 9:
                insert(L"\t");
            break;

            case 13:
                insert(L"\n");
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
                    editor_event event(editor_event::MOVE_LEFT, caret->position);
                    notify(&event);
                }
            break;

            case WXK_RIGHT:
                for (auto &caret : carets) {
                    editor_event event(editor_event::MOVE_RIGHT, caret->position);
                    notify(&event);
                }
            break;

            case WXK_UP:
                for (auto &caret : carets) {
                    editor_event event(editor_event::MOVE_UP, caret->position);
                    notify(&event);
                }
            break;

            case WXK_DOWN:
                for (auto &caret : carets) {
                    editor_event event(editor_event::MOVE_DOWN, caret->position);
                    notify(&event);
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
        _content.insert(caret->position, str);
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


void EditView::notify(event *event) {
    subject::notify(event);
}
