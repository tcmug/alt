
#include "../misc/regtionary.hpp"

class instr;

typedef regtionary <instr*> regtest;

class instr {

    public:

        instr() {
        }

        virtual void apply(const regtest::result &res) {
            std::wstring str(res.at, 0, res.start);
            print(str);
            print(res.snip);
        }

        void print(const std::wstring &str) {
            int n = 0;
            const std::wstring delim = L"\n";
            size_t from = 0, to = 0;
            while (std::wstring::npos != (to = str.find(delim, from))) {
                std::wcout << str.substr(from, to - from) << std::endl;
                from = to + delim.length();
            }
            std::wcout << str.substr(from, to - from);
        }
};


class newline: public instr {

    public:

        newline() {
        }

        virtual void apply(const regtest::result &res) {
            std::wstring str(res.at, 0, res.start);
            print(str);
        }

};

class var: public instr {

    public:

        var() {
        }

        virtual void apply(const regtest::result &res) {
            std::wstring str(res.at, 0, res.start);
            print(str);
            print(L"\e[93m");
            print(res.snip);
            print(L"\e[0m");
        }

};


class op: public instr {

    public:

        op() {
        }

        virtual void apply(const regtest::result &res) {
            std::wstring str(res.at, 0, res.start);
            print(str);
            print(L"\e[92m");
            print(res.snip);
            print(L"\e[0m");
        }

};


class arr: public instr {

    public:

        arr() {
        }

        virtual void apply(const regtest::result &res) {
            std::wstring str(res.at, 0, res.start);
            print(str);
            print(L"\e[91m");
            print(res.snip);
            print(L"\e[0m");
        }


};


class stringi: public instr {

    public:

        stringi() {
        }

        virtual void apply(const regtest::result &res) {
            std::wstring str(res.at, 0, res.start);
            print(str);
            print(L"\e[90m");
            print(res.snip);
            print(L"\e[0m");
        }


};


