
#include "Editor.h"
#include "DrawContext.h"

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Scroll.H>
#include <math.h>

#include <chrono>
#include <algorithm>
#include <memory.h>
#include <map>
#include <string>
#include <iostream>


#define TIMING
#ifdef TIMING
#define INIT_TIMER auto start = std::chrono::high_resolution_clock::now();
#define START_TIMER  start = std::chrono::high_resolution_clock::now();
#define STOP_TIMER(name)  std::cout << "RUNTIME of " << name << ": " << \
    std::chrono::duration_cast<std::chrono::milliseconds>( \
            std::chrono::high_resolution_clock::now()-start \
    ).count() << " ms " << std::endl;
#else
#define INIT_TIMER
#define START_TIMER
#define STOP_TIMER(name)
#endif


bool compareCaret(Caret *a, Caret *b) {
    return (a->_position == b->_position);
}


bool sortCompareCaret(Caret *a, Caret *b) {
    return (a->_position < b->_position);
}

int Editor::handle(int e) {
	int ret = Fl_Widget::handle(e);

	switch (e) {

		case FL_PUSH:
			_mouseDown = true;

			if (_mouseDown) {
				Point caret(Fl::event_x(), Fl::event_y());

				DrawContext ctx = coordinateToContext(caret);

				caret.set(ctx._stopColumn, ctx._stopRow);
				_trackColumn = ctx._stopColumn;
				_trackRow = ctx._stopRow;

				ctx = caretToContext(caret);

				if (!(Fl::event_state() & FL_META)) {
					clearCarets();
				}

				Point pt(
					(-x()) + (ctx._x),
					(-y()) + (ctx._y + fl_descent() - fl_height())
				);

				addCaret(new Caret(
						ctx._position,
						pt,
						Point(
							2,
							ctx._charHeight
						),
						caret
				));

				redraw();
			}

			return 1;
		break;

		case FL_RELEASE:
			_mouseDown = false;
			return 1;
		break;

		case FL_FOCUS:
		case FL_UNFOCUS:
			ret = 1;
			break;

		case FL_PASTE:
			std::cout << Fl::event_text() << std::endl;
			break;

		case FL_KEYDOWN:            // keyboard key pushed
			if (Fl::event_state() & FL_META) {
				switch (Fl::event_text()[0]) {
					case 'v':
						insert("Hello\nworld", 11);
						redraw();
					break;
					case '-':
						if (_fontSize > 5) {
							_fontSize--;
						}
						redraw();
					break;
					case '+':
						if (_fontSize < 100) {
							_fontSize++;
						}
						redraw();
					break;
				}
			}
			else {
				if (Fl::event_key() == FL_Down) {
					down();
				}
				else if (Fl::event_key() == FL_Up) {
					up();
				}
				else if (Fl::event_key() == FL_Left) {
					left();
				}
				else if (Fl::event_key() == FL_Right) {
					right();
				}
				else if (Fl::event_text()[0] == '\r') {
					insert("\n", 1);
				}
				else if (Fl::event_key() == FL_BackSpace) {
					erase(1);
				}
				else {
					insert(Fl::event_text(), Fl::event_length());
				}
				redraw();
			}
			ret = 1;
			break;

		case FL_SHORTCUT:           // incase widget that isn't ours has focus
		case FL_KEYUP:              // keyboard key released
		   // printf("%s\n"s Fl::event_text());
			break;

		case FL_ENTER:
			window()->cursor(FL_CURSOR_INSERT);
			ret = 1;
			break;

		case FL_LEAVE:
            window()->cursor(FL_CURSOR_DEFAULT);
            ret = 1;
            break;

        case FL_DRAG:
		//case FL_MOVE:
			if (_mouseDown) {
				Point caret(Fl::event_x(), Fl::event_y());

				DrawContext ctx = coordinateToContext(caret);

				if (_trackColumn == ctx._stopColumn &&
					_trackRow == ctx._stopRow) {
					ret = 1;
					break;
				}

				caret.set(ctx._stopColumn, ctx._stopRow);

				_trackColumn = ctx._stopColumn;
				_trackRow = ctx._stopRow;

				ctx = caretToContext(caret);

				if (!(Fl::event_state() & FL_META)) {
					clearCarets();
				}

				Point pt(
					(-x()) + (ctx._x),
					(-y()) + (ctx._y + fl_descent() - fl_height())
				);

				addCaret(new Caret(
						ctx._position,
						pt,
						Point(
							2,
							ctx._charHeight
						),
						caret
				));

				redraw();
			}
			ret = 1;
			break;
	}

	return (ret);
}


void Editor::insert(const char *str, size_t length) {

	INIT_TIMER
	START_TIMER

    // Sort carets in the expected format (top to bottom).
    std::sort(_carets.begin(), _carets.end(), sortCompareCaret);
    _carets.erase(unique(_carets.begin(), _carets.end(), compareCaret), _carets.end());

	EditorEvent event(EditorEvent::INSERT_STRING, 0);
	event._string = str;

	bool linestateCleared = false;
	_lineStates.clear();
	for (auto caret : _carets) {

		if (!linestateCleared) {
			DrawContext tctx = positionToContext(caret->_position);
			_lineStates.erase(_lineStates.begin() + (tctx._row - 1), _lineStates.end());
			linestateCleared = true;
		}

		event._position = caret->_position;
		_file.insert(caret->_position, str);
		notify(&event);
	}

	STOP_TIMER("insert")
}


void Editor::erase(size_t length) {

	INIT_TIMER
	START_TIMER

	EditorEvent event(EditorEvent::ERASE_STRING, 0);

	bool linestateCleared = false;
	_lineStates.clear();

	for (auto caret : _carets) {

		if (!linestateCleared) {
			DrawContext tctx = positionToContext(caret->_position);
			_lineStates.erase(_lineStates.begin() + (tctx._row - 1), _lineStates.end());
			linestateCleared = true;
		}

		event._position = caret->_position;
		std::size_t temp_pos = caret->_position;
		int bytes = 0;

		for (int i = 0; i < length && temp_pos > 0; i++) {
			do {
				bytes++;
				temp_pos--;
			} while ((temp_pos > 0) && !IS_PRINTABLE(_file.getContent()[temp_pos]));
		}

		event._string = std::string(_file.getContent(), temp_pos, bytes);
		// std::cout << event._string << std::endl;
		_file.erase(temp_pos, bytes);
		notify(&event);
	}

	STOP_TIMER("erase")
}


void Editor::left() {
	EditorEvent event(EditorEvent::MOVE_LEFT, 0);
	for (auto caret : _carets) {
		event._position = caret->_position;
		notify(&event);
	}
}


void Editor::right() {
	EditorEvent event(EditorEvent::MOVE_RIGHT, 0);
	for (auto caret : _carets) {
		event._position = caret->_position;
		notify(&event);
	}
}


void Editor::up() {
	EditorEvent event(EditorEvent::MOVE_UP, 0);
	for (auto caret : _carets) {
		event._position = caret->_position;
		notify(&event);
	}
}


void Editor::down() {
	EditorEvent event(EditorEvent::MOVE_DOWN, 0);
	for (auto caret : _carets) {
		event._position = caret->_position;
		notify(&event);
	}
}


void Editor::draw() {

	fl_color(color());
	fl_rectf(x(), y(), w(), h());

	fl_font(_font, _fontSize);
	fl_color(7);

	Formatting::Result res = _format->scan(_file.getContent(), _file.getContent() + _file.getLength());

	// No linestates?
	//  - Render all lines
	// Linestates?
	// 	- Find first that is visible
	// 	- Render until end of viewport
	// Insert & Erase?
	//  - Clear states from first changed line until end

	float sx = x();
	float sy = (float)y() - fl_descent() + fl_height();
	int may = 0, max = 0;

	DrawContext ctx(x(), y(), sx, sy, &res, &_lineStates);
	ctx._leftMargin = sx;

	INIT_TIMER
	START_TIMER

	if (_lineStates.size() > 0) {

		int row = 0;
		for (auto &line : _lineStates) {
			if (line._y + y() >= 0) {
				break;
			}
			res = line._result;
			// _result contains wrong string ending here &&
			// it nastily cuts off the rendering and pointer selection
			res.setEnd(_file.getContent() + _file.getLength());
			ctx._y = line._y + y();
			row++;
		}

		ctx._row = row;

		while (res.next()) {
			res.getCurrentNode()->getValue()->print(&ctx);
			// if (ctx._y > parent()->h()) {
			// 	break;
			// }
		}

	}
	else {

		while (res.next()) {
			res.getCurrentNode()->getValue()->print(&ctx);
			may = ctx._y-y();
			if (max < ctx._x-x()) {
				max = ctx._x-x();
			}
		}

		max = 100 * (ceil(max / 100) + 1);
		if (h() != may || w() != max) {
			resize(x(), y(), max, may);
		}

	}

	STOP_TIMER("text render")
	START_TIMER

	for (auto &caret : _carets) {
		if (caret->isDirty()) {
			DrawContext tctx = positionToContext(caret->_position);
			caret->_screen.set(
				(-x()) + (tctx._x),
				(-y()) + (tctx._y + fl_descent() - fl_height())
			);
			caret->_caret.set(tctx._stopColumn, tctx._stopRow);
			caret->markClean();
		}
		caret->render(ctx);
	}

	STOP_TIMER("caret render")

}



DrawContext Editor::coordinateToContext(const Point &coordinate) {

	Formatting::Result res = _format->scan(_file.getContent(), _file.getContent() + _file.getLength());

	float sx = x();
	float sy = (float)y() - fl_descent() + fl_height();

	DrawContext ctx(x(), y(), sx, sy, &res, &_lineStates);

	ctx._render        = false;
	ctx._leftMargin    = sx;
	ctx._stopCondition = DrawContext::COORDINATE;
	ctx._stopX         = coordinate._x;
	ctx._stopY         = coordinate._y;
	ctx._stopExact     = false;

	// TODO: Use linestates here

	if (_lineStates.size() > 0) {

		int row = 1;
		for (auto &line : _lineStates) {
			if (line._y + y() >= 0) {
				break;
			}
			res = line._result;
			ctx._y = line._y + y();
			row++;
		}
		ctx._row = row;

		// Render all lines
		ctx._leftMargin = sx;
	}

	while (res.next()) {
		res.getCurrentNode()->getValue()->print(&ctx);
		if (ctx._stopConditionMet) {
			break;
		}
	}

	if (!ctx._stopConditionMet) {
		// @FIXME: return value should be last line + columns
		return ctx;
	}

	return ctx;
}




DrawContext Editor::caretToContext(const Point &caretPosition) {

	Formatting::Result res = _format->scan(_file.getContent(), _file.getContent() + _file.getLength());

	float sx = x();
	float sy = (float)y() - fl_descent() + fl_height();

	DrawContext ctx(x(), y(), sx, sy, &res, &_lineStates);

	ctx._render        = false;
	ctx._leftMargin    = sx;
	ctx._stopCondition = DrawContext::CARET;
	ctx._stopColumn    = caretPosition._x;
	ctx._stopRow       = caretPosition._y;
	ctx._stopExact     = false;

	while (res.next()) {
		res.getCurrentNode()->getValue()->print(&ctx);
		if (ctx._stopConditionMet) {
			break;
		}
	}

	if (!ctx._stopConditionMet) {
		return ctx;
	}

	return ctx;
}



DrawContext Editor::positionToContext(std::size_t position) {

	Formatting::Result res = _format->scan(_file.getContent());

	float sx = x();
	float sy = (float)y() - fl_descent() + fl_height();

	DrawContext ctx(x(), y(), sx, sy, &res, &_lineStates);

	ctx._render        = false;
	ctx._leftMargin    = sx;
	ctx._stopCondition = DrawContext::POSITION;
	ctx._stopPosition  = position;
	ctx._stopExact     = false;

	std::cout << std::endl;
	if (_lineStates.size() > 0) {
		for (auto &line : _lineStates) {
			line.report();
			if (line._position >= position) {
				break;
			}
		}
	}
	std::cout << std::endl;

	while (res.next()) {
		res.getCurrentNode()->getValue()->print(&ctx);
		if (ctx._stopConditionMet) {
			break;
		}
	}

	if (!ctx._stopConditionMet) {
		return ctx;
	}

	return ctx;
}



Editor::Editor(int X,int Y,int W,int H,const char*L) : Fl_Widget(X,Y,W,H,L) {
	box(FL_FLAT_BOX);
	color(0);

	_mouseDown = false;
	_font = FL_COURIER;
/*
	std::map <std::string, int> fonts;

	int i, k = Fl::set_fonts("-*"), t;
	for (i = 0; i < k; i++) {
		fonts[Fl::get_font_name((Fl_Font)i,&t)] = i;
	}

	auto fi = fonts.find("Roboto Mono");

	if (fi != fonts.end()) {
		_font = fi->second;
	}
	else {
		_font = FL_COURIER;
	}
*/
	_fontSize = 14;
	_file.read("testfile.txt");

	// _content = (char*)malloc(512);
	// strcpy(_content, "Hello->world!!\n日本国 --> x\n\n-- $this->\nroy->日本国->Hello->кошка->кошка->$there - 200;\n\t<?php\n\t\t\"hello\"\n\t?>\nuh->oh\n\n\t1\t2\t3\n\t10\t20\t30\n\t505\t545\t334\n\nThis is the last line.\nHi, it wasn't the last one afterall\t\t\tIT IS THIS ONE.\n\n\nNo -- its this.");
	//strcpy(_content, "\n#include <stdio.h>\n\n/* Comment */\n\nvoid main() {\n}\n");
	_format = new Formatting();
	_format->getRoot()->setValue(new Element(0xA0A0A000));
	ElementNewLine *eol = new ElementNewLine(0xFF00FF00);
	Element *c2 = new Element(0x00FF0000);
	ElementTab *tab = new ElementTab(0x80808000);

	_format->insert("\n", eol);
	_format->insert("\t", tab);
	_format->insert("本", c2);

	_format->insert("Permission", c2);


	/*
	Element *c2 = new Element(0x00FF0000);
	Element *c3 = new Element(0xFFFF0000);
	Element *c4 = new Element(0xFF00FF00);
	Element *c5 = new Element(0x7030FF00);
	ElementNewLine *eol = new ElementNewLine(0xFF00FF00);
	ElementTab *tab = new ElementTab(0x80808000);

	_format->insert("\\n", eol);
	_format->insert("\\t", tab);
	_format->insert("->", c2);
	_format->insert("[0-9]+", c5);
	_format->insert("-->", c3);
	_format->insert("[-+*=\\/]", c4);
*/
	// Formatting::Node *tag = _format->insert("\\<!--", &c8, Formatting::ENTER);
	// tag->insert("[\\r\\n]+", &eol);
	// tag->insert("-->", &c8, Formatting::DROP);

	// Formatting::Node *php = _format->insert("\\<\\?php", &c8, Formatting::ENTER);
	// php->insert("[\\r\\n]+", &eol);
	// php->insert("\\$[a-zA-Z0-9]+", &c2);
	// php->insert("\\?>", &c8, Formatting::DROP);
	// php->insert("\\\"[^\\\"]*\\\"", &c6);
	// php->insert("\\'[^\\']*\\'", &c7);

}


void Editor::clearCarets() {
	for (auto caret : _carets) {
		delete caret;
	}
	_carets.clear();
}


void Editor::addCaret(Caret *caret) {
	_carets.push_back(caret);
	_carets[_carets.size()-1]->subscribe(this);
    std::sort(_carets.begin(), _carets.end());
}


