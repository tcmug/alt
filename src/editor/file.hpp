#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <vector>

class file {

    private:

        class line {
            public:
                line(std::size_t s,  std::size_t l, std::size_t ln): start(s), length(l), line_number(ln) {
                }

                std::size_t start;
                std::size_t length;
                std::size_t line_number;

        };

        std::wstring content;
        std::vector <line> lines;

        void scan_lines(std::size_t pos);

    public:

        const line &operator[] (std::size_t ln);
        std::size_t number_of_lines();

        void read(std::string file);
        std::wstring get_line(std::size_t pos);

        std::size_t position_to_line(std::size_t pos);
        std::size_t position_to_column(std::size_t pos);

        void insert(std::size_t pos, std::wstring str);
        void erase(std::size_t pos, std::size_t len);


    friend struct position_in_file;

};

#endif
