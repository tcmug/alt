#ifndef _EDITOR_DRAWCONTEXT_H_
#define _EDITOR_DRAWCONTEXT_H_

#include "Formatting.h"
#include "Line.h"
#include "Point.h"

class DrawContext {

	public:

		//DrawContext() {}
		DrawContext(float sx, float sy, float x, float y, Formatting::Result *result, LineStates *lines):
			_render(true),
			_sx(sx),
			_sy(sy),
			_x(x),
			_y(y),
			_leftMargin(0),
			_rightMargin(0),
			_topMargin(0),
			_bottomMargin(0),
			_charHeight(0),
			_position(0),
			_column(1),
			_row(1),
			_lineStates(lines),
			_result(result),
			_stopConditionMet(false),
			_stopCondition(NONE),
			_stopPosition(0),
			_stopExact(false),
			_stopColumn(0),
			_stopRow(0),
			_stopX(0),
			_stopY(0) {
		}

		enum StopCondition {
			NONE,
			CARET,
			COORDINATE,
			POSITION
		};

		bool _render;
		float _sx, _sy;
		float _x, _y;
		float _leftMargin, _rightMargin;
		float _topMargin, _bottomMargin;
		float _charHeight;

		std::size_t _position;

		int _column, _row;

		LineStates *_lineStates;
		Formatting::Result *_result;

		bool _stopConditionMet;
		StopCondition _stopCondition;
		std::size_t _stopPosition;
		bool _stopExact;
		int _stopColumn, _stopRow;
		float _stopX, _stopY;

		void print();

};

#endif
