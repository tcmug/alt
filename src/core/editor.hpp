#ifndef EDITOR
#define EDITOR


#include "ide.hpp"
#include "config_dialog.hpp"
#include "wx/wx.h"
#include "wx/sizer.h"

#include "textedit.hpp"

using alt::ide;


class EditView : public wxScrolledWindow {

    protected:

        wxSize canvas_size;

        std::vector <text_line> lines;
        std::vector <text_caret> carets;

        wxSize render(wxDC &dc) const;
        wxPoint lc_to_screen(wxDC &dc, wxPoint &position) const;
        wxPoint screen_to_lc(wxDC &dc, wxPoint &pt) const;

    public:

        EditView(wxFrame* parent);

        void OnChar(wxKeyEvent& event);
        void OnDraw(wxDC &dc);
        void OnLeftClick(wxMouseEvent &event);

        DECLARE_EVENT_TABLE()

};


#endif
