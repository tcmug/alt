#ifndef _EDITOR_EDITOR_H_
#define _EDITOR_EDITOR_H_

#include "Formatting.h"
#include "Caret.h"
#include "Point.h"
#include "Line.h"
#include "File.h"
#include "../misc/Observer.h"

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>


class Editor: public Fl_Widget, public Subject {

	private:

		Formatting *_format;
		int _font;
		int _fontSize;

		File _file;
		LineStates _lineStates;
		std::vector <Caret*> _carets;

		int handle(int e);
		void draw();

		void insert(const char *str, size_t length);

		Point positionToCoordinate(const Point &position);
		Point coordinateToPosition(const Point &position);
		DrawContext coordinateToContext(const Point &coordinate);
		DrawContext positionToContext(const Point &coordinate);

		void clearCarets();
		void addCaret(Caret *caret);

	public:

		// CONSTRUCTOR
		Editor(int X, int Y, int W, int H, const char*L=0);

};

#endif
