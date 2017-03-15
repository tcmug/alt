#ifndef TEXT_RENDER_CONTEXT_HPP
#define TEXT_RENDER_CONTEXT_HPP

#include "wx/wx.h"
#include "../misc/state.hpp"


class root_state: public state <wchar_t> {

    public:

        root_state() {
        }

        virtual void enter(state_stack_type *s) {
        }

        virtual void exit(state_stack_type *s) {
        }

        virtual void handle_input(state_stack_type *s, wchar_t c);

        virtual wxColour get_color() const {
            return wxColour(0, 0, 0);
        }

};



class tag: public root_state {

    private:
        wchar_t previous;

    public:

        tag() {
            previous = 0;
        }

        virtual void enter(state_stack_type *s) {
        }

        virtual void exit(state_stack_type *s) {
        }

        virtual void handle_input(state_stack_type *s, wchar_t c);

        virtual wxColour get_color() const {
            return wxColour(0, 255, 0);
        }


};


class quote: public root_state {

    private:
        wchar_t previous;
        int length;

    public:

        quote() {
            length = 0;
            previous = 0;
        }

        virtual void enter(state_stack_type *s) {
            length = 0;
            previous = 0;
        }

        virtual void exit(state_stack_type *s) {
        }

        virtual void handle_input(state_stack_type *s, wchar_t c);

        virtual wxColour get_color() const {
            return wxColour(0, 0, 255);
        }


};


class text_render_context {
    public:

        text_render_context(wxDC *_dc);
        void print(std::wstring content);
        wxSize get_extents(std::wstring content);

        wxDC *dc;

        wxPoint position;
        wxPoint screen;

        int max_line_width;
        int max_line_height;
        int line_height;
        int left_padding;

        int offset_y;
        int lower_y;

        state_stack <wchar_t> *sstack;

        void report();
        void reset();
};


#endif
