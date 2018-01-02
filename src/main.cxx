
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Sys_Menu_Bar.H>

#include <math.h>
#include <stdio.h>
#include <time.h>

#include "editor/Editor.h"


void Quit_CB(Fl_Widget *, void *) {
    exit(0);
}

// MAIN
int main() {
	 Fl::scheme("gleam");
     Fl_Double_Window win(500, 500);
  //    	Fl_Sys_Menu_Bar *menu = new Fl_Sys_Menu_Bar(0,0,400,25);
		// menu->add("File/Quit",   FL_CTRL+'q', Quit_CB);
		// menu->add("Edit/Change", FL_CTRL+'c', Quit_CB);
		// menu->add("variant/Submenu/Aaa");
		// menu->add("Edit/Submenu/Bbb");

     	Fl_Scroll scroll(0, 0, 500, 500);
            Editor editor(0, 0, 1000, 2000);
        scroll.end();
    win.end();
    win.resizable(editor);
	win.show();
    return (Fl::run());
}
