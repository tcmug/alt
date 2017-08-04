
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



template <typename T>
std::vector <T> split_string(const T string, T delim) {
    std::vector <T> result;
    size_t from = 0, to = 0;
    while (T::npos != (to = string.find(delim, from))) {
        result.push_back(string.substr(from, to - from));
        from = to + delim.length();
    }
    result.push_back(string.substr(from, to));
    return result;
}



#include "editor/instr.hpp"

bool alt_ide_app::OnInit() {

	// regtest dic;

	// dic.root.value = new instr();
	// dic.insert(L"[0-9]+", new var());
	// dic.insert(L"\\$[a-zA-Z0-9]+", new var());
	// dic.insert(L"->", new arr());
	// dic.insert(L"[-+*=\\/]", new op());
	// dic.insert(L"\\\"[^\\\"]*\\\"", new stringi());
	// dic.insert(L"\\'[^\\']*\\'", new stringi());

	// dic.print();

	// std::cout << "***" << std::endl;

	// const wchar_t *code = L"Hello -> world!\nThere 'should' be\n\"SIX\" (6) $lines->count. And 'so' on. 300.\nx = 2 + 3\n'This should be\na multiline string' OK?";
	// std::wstring result;

	// regtest::result res = dic.scan(code, code + wcslen(code));
	// std::wcout << "***" << std::endl << code << std::endl;
	// std::cout << "***" << std::endl;

	// while (res.next()) {
	// 	res.current_node->value->apply(res);
	// }

	// std::cout << "***" << std::endl;
	// std::vector <std::wstring> v = split_string<std::wstring>(code, L"\n");
	// for (auto s : v) {
	// 	std::wcout << s << std::endl;
	// }

	// std::cout << "***" << std::endl;

	if ( !wxApp::OnInit() )
		return false;

	alt::ide* frame = new alt::ide();
	frame->Show(true);

	return true;
}

