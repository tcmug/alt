#ifndef REGTIONARY_HPP
#define REGTIONARY_HPP

#include <wx/regex.h>
#include <string>
#include <vector>

template <class VALUE>
class regtionary {

    public:

        class node;

    public:


        enum NODETYPE {
            ROOT,
            SINGLE,
            ENTER,
            DROP
        };


        class result {
            public:

                result(node *_cn, const wchar_t *_at, const wchar_t *_end):
                    current_node(_cn),
                    at(_at),
                    end(_end),
                    start(0),
                    length(0),
                    hits(0) {
                    if (!end) {
                        end = _at + wcslen(_at);
                    }
                }

                node *current_node;
                int hits;
                const wchar_t *at;
                const wchar_t *end;

                std::wstring snip;

                size_t start, length;

                bool next() {

                    assert(at != NULL);
                    assert(current_node != NULL);

                    if (current_node->type == regtionary <VALUE>::SINGLE) {
                        current_node = current_node->parent;
                    }
                    else if (current_node->type == regtionary <VALUE>::DROP) {
                        current_node = current_node->parent->parent;
                    }


                    at = at + (start + length);

                    if (at >= end || *at == 0) {
                        return false;
                    }

                    start = 100000;
                    length = 0;
                    node *_next = NULL;
                    int max_length = end - at;

                    for (auto item : current_node->items) {
                        if ((*item).regex->Matches(at)) {
                            size_t s, l;
                            (*item).regex->GetMatch(&s, &l, 0);
                            // If this fails, replace <= with <
                            if (s < start && s <= max_length && s <= max_length && (s + l) <= max_length) {
                                _next = item;
                                start = s;
                                length = l;
                            }
                        }
                    }

                    // No match found, return full string as match.
                    if (_next == NULL) {
                        start = end - at;
                        length = 0;
                        snip = L"";
                        return true;
                    }

                    snip = at;
                    snip = snip.substr(start, length);

                    current_node = _next;

                    return true;
                }

        };


        class node {

            public:

                VALUE value;
                NODETYPE type;

                std::wstring re;
                std::vector <node*> items;
                wxRegEx *regex;
                node *parent;

                node() {
                    parent = NULL;
                    type = ROOT;
                }

                node(std::wstring _re, VALUE _value, NODETYPE t) {
                    re = _re;
                    regex = new wxRegEx(re.c_str(), wxRE_ADVANCED);
                    value = _value;
                    type = t;
                }

                virtual void print(int depth = 0) {
                    for (auto item : items) {
                        std::wstring padding(depth * 2, ' ');
                        std::cout << padding << (*item).re << std::endl;
                        if ((*item).items.size() > 0) {
                            (*item).print(depth + 1 );
                        }
                    }
                }

                virtual bool last() {
                    return items.size() == 0;
                }


                node *insert(std::wstring key, VALUE value, NODETYPE type = SINGLE) {
                    node *nn = new node(key, value, type);
                    nn->parent = this;
                    items.push_back(nn);
                    return nn;
                }

        };

        node root;

        node *insert(std::wstring key, VALUE value, NODETYPE type = SINGLE) {
            return root.insert(key, value, type);
        }

        void print() {
            root.print(0);
        }

        result scan(const wchar_t *str, const wchar_t *end = 0) {
            return result(&root, str, end);
        }

        node *get_root() {
            return &root;
        }

};


#endif
