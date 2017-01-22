
#include "ide.hpp"
#include "config_dialog.hpp"
#include "wx/wx.h"
#include "wx/sizer.h"

#include "../editor/editor.hpp"

using alt::ide;

enum {
	Zero_Menu,
	Menu_File_ShowDialog,
	Menu_Blah,
    wxID_Blah
};



ide::ide():
	wxFrame(
		NULL,
		wxID_ANY,
		wxT("Alt"),
		wxDefaultPosition,
		wxSize(420, 300),
		wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE
	)
{
	//SetIcon(wxICON(sample));
	CreateStatusBar(2);
	SetStatusText(wxT("Alt"), 1);
	SetStatusText(wxT("..."), 0);

    EditView *file = new EditView( (wxFrame*) this );

// new wxTextCtrl(this, wxID_Blah, "Hi!", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH , wxDefaultValidator, wxTextCtrlNameStr);


	// view = new wxDataViewCtrl(
	//     this,
	//     wxID_ANY,
	//     wxDefaultPosition,
	//     wxDefaultSize,
	//     wxDV_DEFAULT_STYLE
	// );

	// tr = new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_EDITABLE );
	// wxDataViewColumn *column1 =
	//     new wxDataViewColumn( "artist", tr, 1, 150, wxALIGN_LEFT,
	//     wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE |
	//     wxDATAVIEW_COL_RESIZABLE );

    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(wxID_PREFERENCES);
    fileMenu->Append(wxID_EXIT);

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    SetMenuBar(menuBar);

    Connect(wxID_PREFERENCES,
            wxEVT_MENU,
            wxCommandEventHandler(ide::OnPreferences), NULL, this);

}


ide::~ide() {

}


void ide::OnPreferences(wxCommandEvent&) {
	config_dialog dlg(this);
	dlg.init_by_function("alt_preferences_get", "alt_preferences_set");
	dlg.ShowModal();
}


