#ifndef TEXT_STATE_HPP
#define TEXT_STATE_HPP

#include "wx/wx.h"
#include "../misc/state.hpp"

class tag;
class quote;

class root_state: public state <wchar_t> {

    protected:

        int offset;
        wchar_t previous;

    public:

        root_state() {
            offset = 0;
        }

        virtual void enter(state_stack_type *s) {
        }

        virtual void exit(state_stack_type *s) {
        }

        virtual void handle_input(state_stack_type *s, wchar_t c);

        virtual wxColour get_color() const {
            return wxColour(0, 0, 0);
        }

        int get_offset() const {
            return offset;
        }

};



class tag: public root_state {

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




class arrow: public root_state {

    private:
        int length;

    public:

        arrow() {
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
            return wxColour(0, 255, 255);
        }


};



#endif
