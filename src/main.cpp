
#include "main.hpp"

#include "core/ide.hpp"
#include "core/lua.hpp"

class alt_ide_app: public wxApp {

	public:
		alt_ide_app() {}

	private:

		virtual bool OnInit() wxOVERRIDE;
		wxDECLARE_NO_COPY_CLASS(alt_ide_app);

};



wxIMPLEMENT_APP(alt_ide_app);

bool alt_ide_app::OnInit() {
	if ( !wxApp::OnInit() )
		return false;

	alt::ide* frame = new alt::ide();
	frame->Show(true);

	return true;
}

