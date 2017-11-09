#ifndef EDITOR_EVENT_HPP
#define EDITOR_EVENT_HPP

#include "../misc/observer.hpp"

class editor_event:
    public event {

    public:

        enum TYPE {
            INSERT_LINE,
            INSERT_STRING,
            ERASE_LINE,
            ERASE_STRING,
            MOVE_RIGHT,
            MOVE_LEFT,
            MOVE_UP,
            MOVE_DOWN,
        };

        editor_event(TYPE t, std::size_t pos): type(t), position(pos) {
        }

        TYPE type;
        std::size_t position;
        std::wstring string;
};

#endif
