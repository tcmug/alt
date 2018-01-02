
#include "Caret.h"
#include "DrawContext.h"
#include "Editor.h"

#include <FL/fl_draw.H>
#include <algorithm>

Caret::Caret(std::size_t position, Point screen, Point extents, Point caret):
	_position(position),
	_screen(screen),
	_extents(extents),
	_caret(caret) {
}


void Caret::render(DrawContext &tx) const {

	fl_color(0xFFFF0000);
	fl_rectf(tx._sx + _screen._x, tx._sy + _screen._y, _extents._x, _extents._y);

}


void Caret::update() {

}


void Caret::copyFromContext(DrawContext &ctx) {
	_position = ctx._position;
	_caret.set(ctx._column, ctx._row);
}


bool Caret::operator < (const Caret& other) const {
	return (_position < other._position);
}

bool Caret::operator == (const Caret& other) const {
	return (_position == other._position);
}


void Caret::report() {
}


void Caret::notify(Event *event_in) {

	EditorEvent *event = static_cast<EditorEvent*>(event_in);
	Editor *source = static_cast<Editor*>(event->_source);

	switch (event->_type) {

		case EditorEvent::MOVE_LEFT:
			if (_position == event->_position) {
				if (_position > 0) {
					_position--;
					markDirty();
				}
			}
		break;

		case EditorEvent::MOVE_RIGHT:
			if (_position == event->_position) {
				if (_position < source->_file.getLength() - 1) {
					_position++;
					markDirty();
				}
			}
		break;

		case EditorEvent::MOVE_UP:
			if (_position == event->_position) {
				Point caret(_caret._x, _caret._y - 1);
				DrawContext ctx = source->caretToContext(caret);
				_position = ctx._position;
				markDirty();
			}
		break;

		case EditorEvent::MOVE_DOWN:
			if (_position == event->_position) {
				Point caret(_caret._x, _caret._y + 1);
				DrawContext ctx = source->caretToContext(caret);
				_position = ctx._position;
				markDirty();
			}
		break;
/*
		case EditorEvent::ERASE_STRING:
			if (position >= event->position) {
				if (position > event->string.length())
					position -= event->string.length();
				else
					position = 0;
				//mark_dirty();
			}
		break;
*/
		case EditorEvent::INSERT_STRING:
			if (_position >= event->_position) {
				_position += event->_string.length();
			}
			markDirty();
		break;

	}
}
