
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

#include <wx/regex.h>
#include <string>
#include <map>

#include "misc/regtionary.hpp"

class instr;

typedef	regtionary <instr*> regtest;

class instr {

	public:

		instr() {
		}

		virtual void apply(const regtest::result &res) {
			std::wstring str(res.at, 0, res.start);
			std::cout << str << res.snip;
		}
};


class newline: public instr {

	public:

		newline() {
		}

		virtual void apply(const regtest::result &res) {
			std::wstring str(res.at, 0, res.start);
			std::cout << str << std::endl;
		}

};

class var: public instr {

	public:

		var() {
		}

		virtual void apply(const regtest::result &res) {
			std::wstring str(res.at, 0, res.start);
			std::cout << str;
			std::cout << "\e[93m" << res.snip << "\e[0m";
		}

};


class op: public instr {

	public:

		op() {
		}

		virtual void apply(const regtest::result &res) {
			std::wstring str(res.at, 0, res.start);
			std::cout << str;
			std::cout << "\e[92m" << res.snip << "\e[0m";
		}

};


class arr: public instr {

	public:

		arr() {
		}

		virtual void apply(const regtest::result &res) {
			std::wstring str(res.at, 0, res.start);
			std::cout << str;
			std::cout << "\e[91m" << res.snip << "\e[0m";
		}


};


class stringi: public instr {

	public:

		stringi() {
		}

		virtual void apply(const regtest::result &res) {
			std::wstring str(res.at, 0, res.start);
			std::cout << str;
			std::cout << "\e[90m" << res.snip << "\e[0m";
		}


};


bool alt_ide_app::OnInit() {

	regtest dic;

	dic.root.value = new instr();
	dic.insert(L"[0-9]+", new var());
	dic.insert(L"\\$[a-zA-Z0-9]+", new var());
	dic.insert(L"->", new arr());
	dic.insert(L"[-+*=\\/]", new op());
	// auto tag = dic.insert(L"\\<\\/?\\w+", L"TAG-ENTER", regtionary<std::wstring>::ENTER);
	dic.insert(L"\\\"[^\\\"]*\\\"", new stringi());
	dic.insert(L"\\'[^\\']*\\'", new stringi());
	// tag->insert(L"([a-z]+)(?=\\W*\\=)", L"ATTR");
	// tag->insert(L"\\>", L"TAG-EXIT", regtionary<std::wstring>::DROP);

	// auto entry = dic.insert(L"\\<\\?(php)?", L"PHP-ENTER", regtionary<std::wstring>::ENTER);
	// entry->insert(L"\\?\\>", L"PHP-EXIT", regtionary<std::wstring>::DROP);

	dic.print();

	std::cout << "***" << std::endl;

	const wchar_t *code = L"Hello -> world!\nThere should be\n\"SIX\" (6) $lines->count. And 'so' on. 300.\nx = 2 + 3\n'This should be\na multiline string' OK?";
	std::wstring result;

	regtest::result res = dic.scan(code);
	std::cout << "***" << std::endl;

	while (res.next()) {
		res.current_node->value->apply(res);
	}

	std::wcout << std::endl;

	return 0;

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

