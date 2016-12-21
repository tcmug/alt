#ifndef EDITOR
#define EDITOR


#include "ide.hpp"
#include "config_dialog.hpp"
#include "wx/wx.h"
#include "wx/sizer.h"

#include "text_line.hpp"
#include "text_caret.hpp"
#include "text_render_context.hpp"


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

        int font_size;
        wxFont font;

        wxSize render(wxDC &dc);
        wxPoint lc_to_screen(wxDC &dc, wxPoint &position) const;
        wxPoint screen_to_lc(wxDC &dc, wxPoint &pt) const;

        void insert(std::wstring str);

    public:

        EditView(wxFrame* parent);

        void OnChar(wxKeyEvent& event);
        void OnDraw(wxDC &dc);
        void OnLeftClick(wxMouseEvent &event);

        DECLARE_EVENT_TABLE()

};


#endif
