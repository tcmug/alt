#ifndef EDITOR_HPP
#define EDITOR_HPP


#include "../core/ide.hpp"
#include "wx/wx.h"
#include "wx/sizer.h"

#include "../misc/observer.hpp"
#include "../misc/regtionary.hpp"
#include "file.hpp"
#include "text_caret.hpp"
#include "text_render_context.hpp"
#include "text_marker.hpp"
#include "dirtyable.hpp"
#include "editor_event.hpp"

using alt::ide;


class EditView: public wxScrolledWindow, public dirtyable, public subject {

    friend class text_caret;

    protected:

        std::vector <text_caret*> carets;
        std::vector <text_marker*> markers;
        formatting *format;

        //
        file content;
        std::vector <line_state> line_states;
        wxSize canvas_size;
        //

        int font_size;
        float scale;

        wxFont font;
        wxBitmap *buffer;

        wxPoint paint_start;
        wxPoint paint_end;

        bool paint;

        bool point_on_line(const wxPoint &point, int ln) const;
        wxPoint fix_to_char(wxClientDC &dc, const wxPoint &point, int ln, int &offset) const;

        wxSize render(wxDC &dc);

        text_render_context lc_to_trc(wxPoint position);
        text_render_context pt_to_trc(wxPoint pt);

        /**
         * Insert text to carets positions
         * @param str String to insert
         */
        void insert(std::wstring str);
        void erase();

        void insert_new_line();
        void insert_str(std::wstring str);

        void fix_carets();

        void redraw(wxDC &dc);
        void update();

        void notify(event *event);

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

    public:

        void clear_markers();
        void clear_carets();

        EditView(wxFrame* parent);

        void OnChar(wxKeyEvent& event);

        void OnDraw(wxDC &dc);
        void OnLeftDown(wxMouseEvent &event);
        void OnLeftUp(wxMouseEvent &event);
        void OnMotion(wxMouseEvent &event);

        DECLARE_EVENT_TABLE()

};


#endif
