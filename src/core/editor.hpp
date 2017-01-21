#ifndef EDITOR
#define EDITOR


#include "ide.hpp"
#include "config_dialog.hpp"
#include "wx/wx.h"
#include "wx/sizer.h"

#include "text_line.hpp"
#include "text_caret.hpp"
#include "text_render_context.hpp"
#include "text_marker.hpp"

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


class EditView : public wxScrolledWindow {

    protected:

        wxSize canvas_size;

        std::vector <text_line> lines;
        std::vector <text_caret> carets;
        std::vector <text_marker> markers;

        int font_size;
        wxFont font;

        wxSize render(wxDC &dc);

        text_render_context lc_to_trc(wxPoint position);
        text_render_context pt_to_trc(wxPoint pt);

        void insert(std::wstring str);

    public:

        EditView(wxFrame* parent);

        void OnChar(wxKeyEvent& event);
        void OnDraw(wxDC &dc);
        void OnLeftClick(wxMouseEvent &event);

        DECLARE_EVENT_TABLE()

};


#endif
