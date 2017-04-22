
#include "state.hpp"

tag tag;
quote quote;
arrow arrow;


void root_state::handle_input(state_stack_type *s, wchar_t c) {
    if (c == L'<') {
        s->push(&tag);
    }
    else if (c == L'-') {
        s->push(&arrow);
    }
}


void tag::handle_input(state_stack_type *s, wchar_t c) {
    if (c == L'"') {
        s->push(&quote);
    }
    else if (previous == L'>') {
        s->pop();
    }
    previous = c;
}



void quote::handle_input(state_stack_type *s, wchar_t c) {
    if (previous == L'"' && length > 1) {
        s->pop();
    }
    length++;
    previous = c;
}

void arrow::handle_input(state_stack_type *s, wchar_t c) {
    if (previous == L'-' && c != L'>') {
        s->pop();
    }
    else if (length == 2) {
        s->pop();
    }
    length++;
    previous = c;
}


