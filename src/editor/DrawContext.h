#ifndef _EDITOR_DRAWCONTEXT_H_
#define _EDITOR_DRAWCONTEXT_H_

#include "Formatting.h"
#include "Line.h"
#include "Point.h"

class DrawContext {

    public:

        //DrawContext() {}
        DrawContext(float x, float y, Formatting::Result *result, LineStates *lines):
            _render(true),
            _x(x),
            _y(y),
            _leftMargin(0),
            _rightMargin(0),
            _topMargin(0),
            _bottomMargin(0),
            _result(result),
            _lineStates(lines),
            _column(1),
            _row(1),
            _stopConditionMet(false),
            _stopCondition(NONE),
            _stopColumn(0),
            _stopRow(0),
            _stopX(0),
            _stopY(0) {
        }

        enum StopCondition {
            NONE,
            POSITION,
            COORDINATE
        };

        bool _render;
        float _x, _y;
        float _leftMargin, _rightMargin;
        float _topMargin, _bottomMargin;

        int _column, _row;
        LineStates *_lineStates;
        Formatting::Result *_result;

        bool _stopConditionMet;
        StopCondition _stopCondition;
        int _stopColumn, _stopRow;
        float _stopX, _stopY;

        void print();

};

#endif
