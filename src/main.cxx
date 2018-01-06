
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Sys_Menu_Bar.H>

#include <math.h>
#include <stdio.h>
#include <time.h>

#include "editor/Editor.h"

#include "misc/Dictionary.h"
#include "misc/TokenTree.h"

void Quit_CB(Fl_Widget *, void *) {
    exit(0);
}

// MAIN
int main() {

   //  TokenTree <const char, const char *> tree;

   // try {
   //      tree.insert("hello",   2, "HELLO");
   //      tree.insert("hello",   5, "HELLO");
   //      tree.insert("abba",    4, "ABBA");
   //      tree.insert("absinth", 7, "ABSINTH");
   //  }
   //  catch (std::exception &e) {
   //      std::cout << "Exception" << std::endl;
   //  }

   //  const char *strings[] = {
   //      "abba",
   //      "absinth",
   //      "hello",
   //      "bueller",
   //      "george",
   //      0
   //  };

   //  for (int i = 0; strings[i]; i++) {
   //      try {
   //          std::cout << tree.match(strings[i]) << std::endl;
   //      }
   //      catch (std::exception &e) {
   //          std::cout << "Not found: " << strings[i] << std::endl;
   //      }
   //  }

    Dictionary <const char *> dic;
    dic.insert("a", "a");
    dic.insert("abba", "abba");
    dic.insert("axe", "axe");
    dic.insert("ab", "ab");
    //dic.insert("abb", "abb");
    dic.insert("duplicate", "duplicate");
    dic.insert("duplicated", "duplicae");
    dic.insert("dupli", "dupli");
    dic.print();

    const char *str = "nothing abba axe bob ab abb duplicate dupli axe";
    Dictionary <const char *>::Result res = dic.scan(str);
    std::cout << str << std::endl;
    while (res.next()) {
        if (res.getCurrentNode()->getValue()) {
            std::cout << " > " << res.getCurrentNode()->getValue() << " length: " << res.getLength() << std::endl;
            std::cout << " @ " << res.getAt() << std::endl;
        }
        else {
            std::cout << " > ROOT " << res.getLength() << std::endl;
            std::cout << " @ " << res.getAt() << std::endl;
        }
        std::cout << std::endl;
    }

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
