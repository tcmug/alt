
#include "File.h"
#include <iostream>

#include <fstream>
#include <assert.h>

void File::read(const char *file) {
    std::ifstream t(file);

    t.seekg(0, std::ios::end);
    _content.reserve(t.tellg() * 2);
    std::cout << t.tellg() << " " << _content.size() << std::endl;
    t.seekg(0, std::ios::beg);

    std::string str, eol("\n");
    while (std::getline(t, str)) {
        _content.append(str);
        _content.push_back('\n');
    }

}


const char *File::getContent() const {
	return _content.c_str();
}


void File::insert(std::size_t pos, std::string str) {
    _content.insert(pos, str);
}


void File::erase(std::size_t pos, std::size_t len) {
    assert(pos >= 0 && pos + len < _content.length());
    _content.erase(pos, len);
}


std::size_t File::getLength() const {
    return _content.length();
}
