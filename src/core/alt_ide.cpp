
#include "alt_ide.hpp"

alt_ide::alt_ide():
	wxFrame(
		NULL,
		wxID_ANY,
		wxT("alt"),
    	wxDefaultPosition,
    	wxSize(420, 300),
        wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE
    )
{
    //SetIcon(wxICON(sample));
    CreateStatusBar(2);
	SetStatusText(wxT("Everything"), 1);
}


alt_ide::~alt_ide() {

}
