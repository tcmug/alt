#ifndef _EDITOR_LINE_H_
#define _EDITOR_LINE_H_

#include "Formatting.h"

#include <vector>

class Line {
	public:

		Line(): _start(0), _y(-1), _result() {}

		const char *_start;
		int _y;
		FormattingResult _result;
};

typedef std::vector <Line> LineStates;

#endif
