
#include "file.hpp"

#include <assert.h>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>


void file::read(std::string file) {
    std::wifstream t(file.c_str());

    t.seekg(0, std::ios::end);
    content.reserve(t.tellg());
    std::cout << t.tellg() << " " << content.size() << std::endl;
    t.seekg(0, std::ios::beg);

    std::wstring str, eol(L"\n");
    while (std::getline(t, str)) {
        content.append(str);
        content.push_back('\n');
    }

    scan_lines(0);

}


struct position_in_file {
    position_in_file(std::size_t pos) : position(pos) { }
    bool operator () (const file::line &ln) {
        return (ln.start <= position) && ((ln.start + ln.length) >= position);
    }

    private:
        std::size_t position;
};


void file::scan_lines(std::size_t s) {
    std::size_t start = s, found, ln = position_to_line(s);
    if (ln != std::wstring::npos) {
        // Insert position is within content - find the first line affected.
        lines.erase(lines.begin() + ln, lines.end());
        start = lines.end()->start;
    }
    else {
        // Clear all lines.
        ln = 0;
        start = 0;
        lines.clear();
    }
    std::wstring eol;
    eol.push_back(L'\n');
    while ((found = content.find(eol, start)) != std::string::npos) {
        lines.push_back(line(start, found - start, ln));
        start = found + 1;
        ln++;
    }

}


void file::insert(std::size_t pos, std::wstring str) {
    content.insert(pos, str);
    scan_lines(pos);
}


void file::erase(std::size_t pos, std::size_t len) {
    assert(pos >= 0 && pos + len < content.length());
    content.erase(pos, len);
    scan_lines(pos);
}

const file::line &file::operator[] (std::size_t ln) {
    file::line &rln = lines[ln];
    return rln;
}

std::size_t file::number_of_lines() {
    return lines.size();
}


std::size_t file::position_to_column(std::size_t pos) {
    auto it = std::find_if(lines.begin(), lines.end(), position_in_file(pos));
    if (it != lines.end()) {
        return pos - it->start;
    }
    return std::wstring::npos;
}


std::size_t file::position_to_line(std::size_t pos) {
    auto it = std::find_if(lines.begin(), lines.end(), position_in_file(pos));
    if (it != lines.end()) {
        return it->line_number;
    }
    return std::wstring::npos;
}


std::wstring file::get_line(std::size_t ln) {
    return content.substr(lines[ln].start, lines[ln].length);
}

const std::wstring &file::get_content() {
    return content;
}
