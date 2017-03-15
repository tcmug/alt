
#include "main.hpp"

#include "core/ide.hpp"
#include "core/lua.hpp"

class alt_ide_app: public wxApp {

	public:
		alt_ide_app() {}

	private:

		virtual bool OnInit() wxOVERRIDE;
		wxDECLARE_NO_COPY_CLASS(alt_ide_app);

};


wxIMPLEMENT_APP(alt_ide_app);

#include "misc/dictionary.hpp"

/*


#include "misc/state.hpp"

class root_state: public state <char> {

	public:

		root_state() {
		}

		virtual void enter(state_stack_type *s) {
		}

		virtual void exit(state_stack_type *s) {
		}

		virtual void handle_input(state_stack_type *s, char c);

		virtual const char *get_color() const {
			return "\033[0m";
		}

};



class tag: public root_state {

	private:
		char previous;

	public:

		tag() {
			previous = 0;
		}

		virtual void enter(state_stack_type *s) {
		}

		virtual void exit(state_stack_type *s) {
		}

		virtual void handle_input(state_stack_type *s, char c);

		virtual const char *get_color() const {
			return "\033[32m";
		}

};


class quote: public root_state {

	private:
		char previous;
		int length;

	public:

		quote() {
			length = 0;
			previous = 0;
		}

		virtual void enter(state_stack_type *s) {
			length = 0;
			previous = 0;
		}

		virtual void exit(state_stack_type *s) {
		}

		virtual void handle_input(state_stack_type *s, char c);

		virtual const char *get_color() const {
			return "\033[31m";
		}

};



void root_state::handle_input(state_stack_type *s, char c) {
	if (c == '<') {
		s->push(new tag());
	}
}


void tag::handle_input(state_stack_type *s, char c) {
	if (c == '"') {
		s->push(new quote());
	}
	else if (previous == '>') {
		delete s->pop();
	}
	previous = c;
}



void quote::handle_input(state_stack_type *s, char c) {
	if (previous == '"' && length > 1) {
		delete s->pop();
	}
	length++;
	previous = c;
}
*/


bool alt_ide_app::OnInit() {

	// root_state *st = new root_state();
	// state_stack <char> stack(st);

	// std::string str("<tag attr=\"value\" another=\"\">then some content</tag>");

	// for (int i = 0; i < str.length(); i++) {
	// 	stack.handle_input(str[i]);
	// 	const root_state *state = static_cast <const root_state*> (stack.current());
	// 	std::cout << state->get_color() << str[i];
	// }

	// std::cout << std::endl;


/*
	alt::dictionary <std::string*> blah;

	std::string a = "Matched bob";
	std::string b = "Matched crap";

	blah.add("bob", &a);
	blah.add("crap", &b);

	std::cout << *blah.scan_string("bob", 0) << std::endl;
*/


	if ( !wxApp::OnInit() )
		return false;

	alt::ide* frame = new alt::ide();
	frame->Show(true);

	return true;
}

