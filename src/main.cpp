
#include "main.hpp"

#include "core/ide.hpp"
#include "core/lua.hpp"

class alt_ide_app: public wxApp {

	public:
		alt_ide_app() {}

	private:

		virtual bool OnInit(); //wxOVERRIDE;
		wxDECLARE_NO_COPY_CLASS(alt_ide_app);

};


wxIMPLEMENT_APP(alt_ide_app);


/*


c#include "misc/state.hpp"

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

#include <wx/regex.h>
#include <string>
#include <map>

#include "misc/regtionary.hpp"



bool alt_ide_app::OnInit() {

	regtionary <std::wstring> dic;


	auto tag = dic.insert(L"\\<\\/?\\w+", L"TAG-ENTER", regtionary<std::wstring>::ENTER);
	tag->insert(L"\\\"[^\\\"]*\\\"", L"DQ-STRING");
	tag->insert(L"\\'[^\\']*\\'", L"SQ-STRING");
	tag->insert(L"([a-z]+)(?=\\W*\\=)", L"ATTR");
	tag->insert(L"\\>", L"TAG-EXIT", regtionary<std::wstring>::DROP);

	auto entry = dic.insert(L"\\<\\?(php)?", L"PHP-ENTER", regtionary<std::wstring>::ENTER);
	entry->insert(L"\\?\\>", L"PHP-EXIT", regtionary<std::wstring>::DROP);
	entry->insert(L"\\$[A-Za-z0-9_]+", L"VAR");

	dic.print();

	std::cout << "***" << std::endl;

	const wchar_t *code = L"<tag><another><third><?php if ($then) { $name=123; $Blah_x =4; } ?><html yes single='single' other= \"hop\" id =\"t<>his\"><? if ($var) ?></html>";
	std::wstring result;

	for (int i = 0; i < 1; i++) {
		regtionary <std::wstring>::result res = dic.scan(code);

		while (res.next()) {
			std::wstring ln(res.at);
			if (res.start > 0) {
				result += ln.substr(0, res.start);
				result += L"|";
			}
			if (res.snip.length() > 0) {
				result += res.snip;
				result += L"||";
			}
		}

	}
	//if (result == code) {
	std::wcout << code << std::endl;
	std::wcout << result << std::endl;
	//}
	//return 0;
	// std::cout << dic.find(L"->") << std::endl;
	// std::cout << dic.find(L"-") << std::endl;
	// std::cout << dic.find(L"x") << std::endl;
	// std::cout << dic.find(L"$asdasd22-") << std::endl;

	// std::cout << dic.find(L"Hello world") << std::endl;
	// std::cout << dic.find(L"Hello book") << std::endl;
	// std::cout << dic.find(L"Hello") << std::endl;

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

