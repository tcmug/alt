#ifndef _EDITOR_CARET_H_
#define _EDITOR_CARET_H_

#include "Formatting.h"
#include "DrawContext.h"
#include "EditorEvent.h"
#include "Point.h"
#include "Dirtyable.h"

class Caret: public Observer, public Dirtyable {

	public:

		Caret(std::size_t position, Point screen, Point extents, Point caret);
		void render(DrawContext &tx) const;

		std::size_t _position;
		Point _screen;
		Point _extents;
		Point _caret;

		void copyFromContext(DrawContext &ctx);

		bool operator < (const Caret& other) const;
		bool operator == (const Caret& other) const;

		void report();
		void update();

		void notify(Event *_event);

};

#endif
