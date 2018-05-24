#ifndef _EDITOR_LINE_H_
#define _EDITOR_LINE_H_

#include "Formatting.h"

#include <vector>

class Line {
	public:

		Line(): _position(0), _y(-1), _result(), _row(-1) {
		}

		Line(int position, int y, FormattingResult result, int row): _position(position), _y(y), _result(result), _row(row) {
		}

		int _position;
		int _y;
		int _row;
		int _column;
		FormattingResult _result;

		void report() {
			std::cout << _row << "\t" << _y << "\t" << _position << std::endl;
		}
};

typedef std::vector <Line> LineStates;

#endif
