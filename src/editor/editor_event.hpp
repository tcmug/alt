#ifndef EDITOR_EVENT_HPP
#define EDITOR_EVENT_HPP

#include "../misc/observer.hpp"

class editor_event:
    public event {

    public:

        enum TYPE {
            INSERT_LINE,
            INSERT_STRING
        };

        editor_event(TYPE t, wxPoint pos): type(t), position(pos) {
        }

        TYPE type;
        wxPoint position;
        std::wstring string;
};

#endif
