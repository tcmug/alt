
#include "main.hpp"

#include "core/alt_ide.hpp"


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

	alt_ide* frame = new alt_ide;
	frame->Show(true);

	return true;
}

