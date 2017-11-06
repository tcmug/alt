//
// FLTK drawing example showing simple line drawing animation
// erco 03/22/07
//
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_draw.H>
#include <FL/Fl_Scroll.H>

#include <math.h>
#include <stdio.h>
#include <time.h>

#define BG_COLOR   45
#define TICK_COLOR 50
#define CIRC_COLOR 0

class MyTimer : public Fl_Widget {

	private:

	    void draw() {
	        // COMPUTE NEW COORDS OF LINE
	        static long start = time(NULL);
	        int ddx = 0, ddy = 0, ww = 0, hh = 0;
	        long tick = time(NULL) - start;
	        char secs[80]; sprintf(secs, u8"%02ld:%02ld 必要", tick/60, tick%60);
	        //const char *secs = u8"必要";//u8"Hello World: \U0010FFFF";
	        float pi = 3.14 - (((tick % 60) / 60.0) * 6.28);
	        int radius = h() / 2;
	        int x1 = (int)(x() + w()/2),
	            y1 = (int)(y() + h()/2),
	            x2 = (int)(x1 + (sin(pi) * radius)),
	            y2 = (int)(y1 + (cos(pi) * radius));

			fl_color(color());
	        fl_rectf(x(),y(),w(),h());

	        // DRAW 'SECOND HAND' OVER WIDGET'S BACKGROUND
	        fl_color(TICK_COLOR);
	        fl_line(x1, y1, x2, y2);
	        fl_color(CIRC_COLOR);
	        fl_pie(x1-10, y1-10, 20, 20, 0.0, 360.0);
	        //fl_rectf(0, 0, 20, 20, 255, 0, 255);
	        // DRAW TIMER TEXT STRING
	        fl_color(TICK_COLOR);
	        fl_font(FL_HELVETICA, 13);
	        fl_draw(secs, x()+4, y()+h()-4);

	        fl_text_extents(secs, ddx, ddy, ww, hh);
	       	printf("%i %i %i %i\n", ddx, ddy, ww, hh);

	    }

	    static void Timer_CB(void *userdata) {
	        MyTimer *o = (MyTimer*)userdata;
	        o->redraw();
	        Fl::repeat_timeout(0.25, Timer_CB, userdata);
	    }

	public:
	    // CONSTRUCTOR
	    MyTimer(int X,int Y,int W,int H,const char*L=0) : Fl_Widget(X,Y,W,H,L) {
	        box(FL_FLAT_BOX);
	        color(BG_COLOR);
	        Fl::add_timeout(0.25, Timer_CB, (void*)this);
	    }
};
// MAIN
int main() {
     Fl_Double_Window win(220, 220);
     	Fl_Scroll OhNo(0, 0, 220, 220);
     		MyTimer tim(0, 0, 1000, 1000);
     win.resizable(win);
     win.show();
     return(Fl::run());
}

// #include <FL/Fl.H>
// #include <FL/Fl_Window.H>
// #include <FL/Fl_Box.H>

// int main(int argc, char **argv) {

//   Fl_Window *window = new Fl_Window(300,180);
//   Fl_Box *box = new Fl_Box(20,40,260,100,"Hello, World!");
//   box->box(FL_UP_BOX);
//   box->labelsize(36);
//   box->labelfont(FL_BOLD+FL_ITALIC);
//   box->labeltype(FL_SHADOW_LABEL);
//   window->end();
//   window->show(argc, argv);
//   return Fl::run();

// }

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

