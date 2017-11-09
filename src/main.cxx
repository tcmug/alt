
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_draw.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Sys_Menu_Bar.H>

#include <math.h>
#include <stdio.h>
#include <time.h>

#include "editor/editor.hpp"


void Quit_CB(Fl_Widget *, void *) {
    exit(0);
}

// MAIN
int main() {
	 Fl::scheme("gleam");
     Fl_Double_Window win(220, 220);
     	Fl_Sys_Menu_Bar *menu = new Fl_Sys_Menu_Bar(0,0,400,25);
		menu->add("File/Quit",   FL_CTRL+'q', Quit_CB);
		menu->add("Edit/Change", FL_CTRL+'c', Quit_CB);
		menu->add("variant/Submenu/Aaa");
		menu->add("Edit/Submenu/Bbb");

     	Fl_Scroll OhNo(0, 0, 220, 220);
     	Editor tim(0, 0, 1000, 1000);
     	win.resizable(win);
	win.end();
	win.show();
    return (Fl::run());
}
