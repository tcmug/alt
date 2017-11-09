#ifndef _editor_editor_hpp_
#define _editor_editor_hpp_

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include "element.hpp"
#include "../misc/regtionary.hpp"

typedef regtionary <element> formatting;

class Editor: public Fl_Widget {

    private:

        formatting *_format;
        int _font;

        int handle(int e);
        void draw();

        static void Timer_CB(void *userdata);

    public:
        // CONSTRUCTOR
        Editor(int X,int Y,int W,int H,const char*L=0);

};

#endif
