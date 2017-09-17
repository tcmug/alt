
#include "main.hpp"

#include "core/ide.hpp"
#include "core/lua.hpp"

class variant_app: public wxApp {

	public:
		variant_app() {}

	private:

		virtual bool OnInit(); //wxOVERRIDE;
		wxDECLARE_NO_COPY_CLASS(variant_app);

};

wxIMPLEMENT_APP(variant_app);

#include <wx/regex.h>
#include <string>
#include <map>

bool variant_app::OnInit() {

	if ( !wxApp::OnInit() )
		return false;

	alt::ide* frame = new alt::ide();
	frame->Show(true);

	return true;
}

