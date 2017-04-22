
#include <wx/regex.h>
#include <string>
#include <map>

template <class VALUE>
class dictionary {

    protected:

        class node {
            public:
                VALUE value;
                std::map <wchar_t, node*> items;

                virtual void print(int depth = 0) {
                    for (auto item = items.begin(); item != items.end(); item++) {
                        std::wstring padding(depth * 2, ' ');
                        std::cout << padding << std::wstring(1, item->first) << std::endl;
                        if (item->second->value.length() > 0) {
                            std::cout << padding << "-> " << item->second->value << std::endl;
                        }
                        if (item->second->items.size() > 0) {
                            item->second->print(depth + 1 );
                        }
                    }
                }

                virtual node *match(wchar_t chr) {
                    //std::cout << "? => " << std::wstring(1, chr) << std::endl;
                    auto item = items.find(chr);
                    if (item != items.end()) {
                        return item->second;
                    }
                    return 0;
                }

                virtual bool last() {
                    return items.size() == 0;
                }

        };

        class node_alnum: public node {
            public:

                virtual node *match(wchar_t chr) {
                    node *n = node::match(chr);
                    if (n == 0 && isalnum(chr)) {
                        return this;
                    }
                    return n;
                }

                bool last() {
                    return false;
                }

        };

        node root;

    public:

        class result {
            public:

                result(node *_at, int _length): at(_at), length(_length) {}

                node *at;
                int length;

                bool match() {
                    return length != -1;
                }

        };

        void insert(std::wstring key, VALUE value) {
            node *n = &root;
            for (std::size_t i = 0; i < key.length(); i++) {
                auto item = n->items.find(key[i]);
                if (item != n->items.end()) {
                    n = item->second;
                }
                else {
                    node *nn = new node();
                    n->items[key[i]] = nn;
                    n = nn;
                }
            }
            n->value = value;
        }


        void insert_alnum(std::wstring key, VALUE value) {
            node *n = &root;
            for (std::size_t i = 0; i < key.length(); i++) {
                auto item = n->items.find(key[i]);
                if (item != n->items.end()) {
                    n = item->second;
                }
                else {
                    node *nn = new node_alnum();
                    n->items[key[i]] = nn;
                    n = nn;
                }
            }
            n->value = value;
        }

        void print() {
            root.print(0);
        }

        result find(std::wstring str) {
            node *n = &root;
            node *last = &root;
            std::size_t last_i = -1;
            for (std::size_t i = 0; i < str.length(); i++) {
                auto nn = n->match(str[i]);
                if (nn) {
                    if (nn->last()) {
                        return result(nn, i);
                    }
                    last = nn;
                    last_i = i;
                    n = nn;
                } else {
                    break;
                }
            }
            return result(last, last_i);
        }

};
