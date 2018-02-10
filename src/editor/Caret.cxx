
#include "Caret.h"
#include "DrawContext.h"
#include "Editor.h"

#include <FL/fl_draw.H>
#include <algorithm>
#include <iostream>

Caret::Caret(std::size_t position, Point screen, Point extents, Point caret):
	_position(position),
	_screen(screen),
	_extents(extents),
	_caret(caret) {

	// std::cout << _position << " @ ";
	// std::cout << _caret._x << ":" << _caret._y << " ";
	// std::cout << _screen._x << ":" << _screen._y << " ";
	// std::cout << std::endl;
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

#define IS_PRINTABLE(c) ((static_cast<unsigned char>(c) < 0x80) || (static_cast<unsigned char>(c) > 0xBF))



void Caret::notify(Event *event_in) {

	EditorEvent *event = static_cast<EditorEvent*>(event_in);
	Editor *source = static_cast<Editor*>(event->_source);

	switch (event->_type) {

		case EditorEvent::MOVE_LEFT:
			if (_position == event->_position) {
				do {
					_position--;
				} while ((_position > 0) && !IS_PRINTABLE(source->_file.getContent()[_position]));
				markDirty();
			}
		break;

		case EditorEvent::MOVE_RIGHT:
			if (_position == event->_position) {
				do {
					_position++;
				} while ((_position < source->_file.getLength() - 1) && !IS_PRINTABLE(source->_file.getContent()[_position]));
				markDirty();
			}
		break;

		case EditorEvent::MOVE_UP:
			if (_position == event->_position && _caret._y > 1) {
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

		case EditorEvent::ERASE_STRING:
			if (_position >= event->_position) {
				_position -= event->_string.length();
				markDirty();
			}
		break;

		case EditorEvent::INSERT_STRING:
			if (_position >= event->_position) {
				_position += event->_string.length();
			}
			markDirty();
		break;

	}
}
