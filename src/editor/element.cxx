
#include "DrawContext.h"
#include "Element.h"

#include <FL/fl_draw.H>
#include <math.h>
#include <iostream>

#define IS_PRINTABLE(c) ((static_cast<unsigned char>(c) < 0x80) || (static_cast<unsigned char>(c) > 0xBF))

size_t Element::_determineLength(DrawContext *ctx) {

	const char *at = ctx->_result->getAt();
	size_t length = ctx->_result->getLength();

	switch (ctx->_stopCondition) {
		case DrawContext::POSITION: {
			// Not on this row?
			if (ctx->_row != ctx->_stopRow)
				break;
			// Loop until correct column.
			size_t i, col = ctx->_column;
			for (i=0; i < length; i++) {
				if (IS_PRINTABLE(at[i])) {
					col++;
					if (col == ctx->_stopColumn) {
						break;
					}
				}
			}
			ctx->_stopConditionMet = true;
			length = i;
		}
		break;

		case DrawContext::COORDINATE: {
			// Not on this row?
			if (ctx->_stopY >= ctx->_y) {
				break;
			}
			// Scan x compoment by iterating over the string.
			size_t col = ctx->_column - 1, a = 0, tempX = ctx->_x;
			for (size_t i = 0; i < length + 1; i++) {
				if (IS_PRINTABLE(at[i])) {
					tempX += fl_width(&at[a], i-a);
					a = i;
					if (ctx->_stopX < tempX) {
						// Got a match here.
						length = i;
						ctx->_stopColumn = col;
						ctx->_stopRow = ctx->_row;
						ctx->_stopConditionMet = true;
						break;
					}
					col++;
				}
			}
			ctx->_column = col;

		}
		break;
	}

	return length;
}


void Element::print(DrawContext *ctx) {

	const char *at = ctx->_result->getAt();
	size_t length = _determineLength(ctx);

	if (ctx->_render) {
		fl_color(_color);
		fl_draw(at, length, ctx->_x, ctx->_y);
	}

	ctx->_x += fl_width(at, length);
}


void ElementNewLine::print(DrawContext *ctx) {

	const char *at = ctx->_result->getAt();
	size_t length = _determineLength(ctx);

	if (ctx->_render) {
		fl_color(_color);
		fl_draw("\u2424", ctx->_x, ctx->_y);
	}

	ctx->_row++;
	ctx->_column = 1;
	ctx->_x = ctx->_leftMargin;
	ctx->_y += fl_height();
}


size_t ElementTab::_determineLength(DrawContext *ctx) {

	const char *at = ctx->_result->getAt();
	size_t length = ctx->_result->getLength();

	float rightEdge = (floor(ctx->_x / 50.0) + 1) * 50;

	switch (ctx->_stopCondition) {
		case DrawContext::POSITION: {
		}
		break;

		case DrawContext::COORDINATE: {
			// Not on this row?
			if (ctx->_stopY >= ctx->_y) {
				break;
			}
			if (ctx->_stopX >= ctx->_x && ctx->_stopX < rightEdge) {
				ctx->_stopConditionMet = true;
				ctx->_stopColumn = ctx->_column;
				ctx->_stopRow = ctx->_row;
			}
			ctx->_column++;
		}
		break;
	}

	return length;

}


void ElementTab::print(DrawContext *ctx) {

	const char *at = ctx->_result->getAt();
	size_t length = _determineLength(ctx);

	// if (ctx->_render) {
	//     fl_color(_color);
	//     fl_draw(">>", ctx->_x, ctx->_y);
	// }

	ctx->_x = round((floor(ctx->_x / 50.0) + 1) * 50);
}

