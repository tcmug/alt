
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

int main(int argc, char **argv) {

  Fl_Window *window = new Fl_Window(300,180);
  Fl_Box *box = new Fl_Box(20,40,260,100,"Hello, World!");
  box->box(FL_UP_BOX);
  box->labelsize(36);
  box->labelfont(FL_BOLD+FL_ITALIC);
  box->labeltype(FL_SHADOW_LABEL);
  window->end();
  window->show(argc, argv);
  return Fl::run();

}

// #include "main.hpp"

// #include "core/ide.hpp"
// #include "core/lua.hpp"

// class variant_app: public wxApp {

// 	public:
// 		variant_app() {}

// 	private:

// 		virtual bool OnInit(); //wxOVERRIDE;
// 		wxDECLARE_NO_COPY_CLASS(variant_app);

// };

// wxIMPLEMENT_APP(variant_app);

// #include <wx/regex.h>
// #include <string>
// #include <map>

// bool variant_app::OnInit() {

// 	if ( !wxApp::OnInit() )
// 		return false;

// 	alt::ide* frame = new alt::ide();
// 	frame->Show(true);

// 	return true;
// }

