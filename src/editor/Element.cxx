
#include "DrawContext.h"
#include "Element.h"

#include <FL/fl_draw.H>
#include <math.h>
#include <iostream>


#define IS_PRINTABLE(c) ((static_cast<unsigned char>(c) < 0x80) || (static_cast<unsigned char>(c) > 0xBF))



void Element::print(DrawContext *ctx) {

	ctx->_charHeight = fl_height();
	const char *at = ctx->_result->getAt();
	size_t length = ctx->_result->getLength();

	switch (ctx->_stopCondition) {

		case DrawContext::NONE:
		break;

		case DrawContext::POSITION: {
			// Not on this row?
			if (ctx->_row != ctx->_stopRow)
				break;
			// Loop until correct column.
			for (size_t i = 0; i < length; i++) {
				if (IS_PRINTABLE(at[i])) {
					if (ctx->_column == ctx->_stopColumn) {
						ctx->_stopConditionMet = true;
						length = i;
						break;
					}
					ctx->_column++;
				}
			}
		}
		break;

		case DrawContext::COORDINATE: {
			// Not on this row?
			if (ctx->_stopY >= ctx->_y)
				break;
			// Scan x compoment by iterating over the string.
			// Start at 1
			size_t col = ctx->_column, a = 0;
			float tempX = ctx->_x;
			for (size_t i = 1; i < length + 1; i++) {
				if (IS_PRINTABLE(at[i])) {
					float charWidth = fl_width(&at[a], i-a);
					float offset = ctx->_stopExact ? 0 : (charWidth / 2);
					tempX += charWidth;
					a = i;
					if (ctx->_stopX < tempX - offset) {
						length = i - 1;
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

	if (ctx->_render) {
		fl_color(_color);
		fl_draw(at, length, ctx->_x, ctx->_y);
	}

	ctx->_x += fl_width(at, length);
}




void ElementNewLine::print(DrawContext *ctx) {

	size_t length = ctx->_result->getLength();

	switch (ctx->_stopCondition) {

		case DrawContext::NONE:
		break;

		case DrawContext::POSITION: {
			// Not on this row?
			if (ctx->_row != ctx->_stopRow)
				break;

			ctx->_stopConditionMet = true;
			ctx->_stopColumn = ctx->_column;
			ctx->_stopRow = ctx->_row;

			length = 0;
		}
		return;

		case DrawContext::COORDINATE: {
			// Not on this row?
			if (ctx->_stopY >= ctx->_y)
				break;

			ctx->_stopConditionMet = true;
			ctx->_stopColumn = ctx->_column;
			ctx->_stopRow = ctx->_row;
			ctx->_column++;
		}
		break;
	}

	// if (ctx->_render) {
	// 	fl_color(_color);
	// 	fl_draw("\u2424", ctx->_x, ctx->_y);
	// }

	ctx->_row++;
	ctx->_column = 1;
	ctx->_x = ctx->_leftMargin;
	ctx->_y += fl_height();
}




void ElementTab::print(DrawContext *ctx) {

	const char *at = ctx->_result->getAt();
	size_t length = ctx->_result->getLength();

	float charWidth = 50;
	float rightEdge = ctx->_x + charWidth;
	float offset = ctx->_stopExact ? 0 : charWidth / 2;

	switch (ctx->_stopCondition) {

		case DrawContext::NONE:
		break;

		case DrawContext::POSITION: {
			// Not on this row?
			if (ctx->_row != ctx->_stopRow)
				break;

			if (ctx->_column == ctx->_stopColumn) {
				ctx->_stopConditionMet = true;
				ctx->_stopColumn = ctx->_column;
				ctx->_stopRow = ctx->_row;
				length = 0;
			}
			else {
				ctx->_column++;
			}
		}
		break;

		case DrawContext::COORDINATE: {
			// Not on this row?
			if (ctx->_stopY >= ctx->_y)
				break;
			if (ctx->_stopX < rightEdge - offset) {
				ctx->_stopConditionMet = true;
				ctx->_stopColumn = ctx->_column;
				ctx->_stopRow = ctx->_row;
			}
			ctx->_column++;
		}
		break;
	}

	ctx->_x = ctx->_x + (length * 50);
}

