#ifndef _EDITOR_EDITOR_EVENT_HPP_
#define _EDITOR_EDITOR_EVENT_HPP_

#include "../misc/Observer.h"

#include <string>

class EditorEvent: public Event {

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

		EditorEvent(TYPE t, std::size_t pos): _type(t), _position(pos) {
		}

		TYPE _type;
		std::size_t _position;
		std::string _string;
};

#endif
