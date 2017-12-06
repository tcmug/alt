#ifndef _EDITOR_ELEMENT_HPP_
#define _EDITOR_ELEMENT_HPP_

#include <FL/Fl.H>

class DrawContext;

class Element {

	protected:

		Fl_Color _color;
		Fl_Color _background;

	public:

		Element(): _color(0), _background(0) {}
		Element(Fl_Color color): _color(color) {}

		virtual void print(DrawContext *ctx);

};

class ElementNewLine: public Element {

	protected:

	public:

		ElementNewLine(): Element() {}
		ElementNewLine(Fl_Color color): Element(color) {}

		void print(DrawContext *ctx);

};


class ElementTab: public Element {
	protected:

	public:

		ElementTab(): Element() {}
		ElementTab(Fl_Color color): Element(color) {}

		void print(DrawContext *ctx);

};

#endif
