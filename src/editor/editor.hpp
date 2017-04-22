#ifndef EDITOR_HPP
#define EDITOR_HPP


#include "../core/ide.hpp"
#include "wx/wx.h"
#include "wx/sizer.h"

#include "text_line.hpp"
#include "text_caret.hpp"
#include "text_render_context.hpp"
#include "text_marker.hpp"
#include "dirtyable.hpp"
#include "state.hpp"

using alt::ide;


class edit_event {

    public:

        enum TYPE {
            DELETE,
            INSERT
        };

        TYPE type;
        std::string content;
        std::vector <text_caret> carets;
};


typedef state_stack <wchar_t> state_stack_editor;


class EditView : public wxScrolledWindow, public dirtyable {

    protected:

        wxSize canvas_size;

        std::vector <text_line> lines;
        std::vector <text_caret> carets;
        std::vector <text_marker> markers;

        state_stack<wchar_t> *sstack;

        int font_size;
        float scale;

        wxFont font;
        wxBitmap *buffer;

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

    public:

        EditView(wxFrame* parent);

        void OnChar(wxKeyEvent& event);
        void OnDraw(wxDC &dc);
        void OnLeftDown(wxMouseEvent &event);
        void OnLeftUp(wxMouseEvent &event);

        DECLARE_EVENT_TABLE()

};


#endif
