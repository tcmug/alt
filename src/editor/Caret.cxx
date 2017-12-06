
#include "Caret.h"
#include "DrawContext.h"
#include "Editor.h"

#include <FL/fl_draw.H>
#include <algorithm>

Caret::Caret(std::size_t position, Point screen, Point extents):
	_screen(screen),
	_position(position),
	_extents(extents) {
}


void Caret::render(DrawContext &tx) const {

	fl_color(0xFFFF0000);
	fl_rectf(tx._sx + _screen._x, tx._sy + _screen._y, _extents._x, _extents._y);

}


void Caret::update() {

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
	Editor *source = static_cast<Editor*>(event->source);

	switch (event->type) {

		case EditorEvent::MOVE_LEFT:
			if (_position == event->position) {
				if (_position > 0) {
					_position--;
				}
				//mark_dirty();
			}
		break;
/*
		case EditorEvent::MOVE_RIGHT:
			if (position == event->position) {
				if (position < source->content.get_length() - 1) {
					position++;
				}
				//mark_dirty();
			}
		break;

		case EditorEvent::MOVE_UP:
			if (position == event->position) {
				std::size_t line = source->content.position_to_line(position);
				std::size_t col = source->content.position_to_column(position);
				if (line > 0) {
					line--;
					position = source->content.line_to_position(line) + std::min(col, source->content.get_line_length(line));
					//mark_dirty();
				}
			}
		break;

		case EditorEvent::MOVE_DOWN:
			if (position == event->position) {
				std::size_t line = source->content.position_to_line(position);
				std::size_t col = source->content.position_to_column(position);
				if (line < source->content.number_of_lines() - 1) {
					line++;
					position = source->content.line_to_position(line) + std::min(col, source->content.get_line_length(line));
					//mark_dirty();
				}
			}
		break;

		case EditorEvent::ERASE_STRING:
			if (position >= event->position) {
				if (position > event->string.length())
					position -= event->string.length();
				else
					position = 0;
				//mark_dirty();
			}
		break;

		case EditorEvent::INSERT_STRING:
			if (position >= event->position) {
				position += event->string.length();
				//mark_dirty();
			}
		break;
	*/
	}
}
