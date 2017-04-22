#ifndef REGTIONARY_HPP
#define REGTIONARY_HPP


template <class VALUE>
class regtionary {

    protected:
        class node;

    public:


        enum NODETYPE {
            SINGLE,
            ENTER,
            DROP
        };


        class result {
            public:

                result(node *_cn, const wchar_t *_at):
                    current_node(_cn),
                    at(_at),
                    start(0),
                    length(0) {
                }

                node *current_node;
                const wchar_t *at;

                std::wstring snip;

                size_t start, length;

                bool next() {

                    assert(at != NULL);
                    assert(current_node != NULL);

                    at = at + (start + length);

                    if (*at == 0) {
                        return false;
                    }

                    start = 1000;
                    length = 0;
                    node *_next = NULL;

                    for (auto item : current_node->items) {
                        if ((*item).regex->Matches(at)) {
                            size_t s, l;
                            (*item).regex->GetMatch(&s, &l, 0);
                            if (s < start) {
                                snip = at;
                                snip = snip.substr(s, l);
                                _next = item;
                                start = s;
                                length = l;
                            }
                        }
                    }

                    if (_next == NULL) {
                        std::wcout << at << std::endl;
                        start = wcslen(at);
                        length = 0;
                        snip = L"";
                        return true;
                    }

                    switch (_next->type) {
                        case regtionary <std::wstring>::ENTER:
                            //std::wcout << L"ENTER " << _next->value << std::endl;
                            current_node = _next;
                        break;
                        case regtionary <std::wstring>::DROP:
                            //std::wcout << L"DROP TO " << current_node->parent->value << std::endl;
                            current_node = current_node->parent;
                        break;
                        default:
                        break;
                    }

                    return true;
                }

        };


    protected:

        class node {

            public:

                VALUE value;
                NODETYPE type;

                std::wstring re;
                std::vector <node*> items;
                wxRegEx *regex;
                node *parent;

                node() {
                    value = L"ROOT";
                    parent = NULL;
                    type = SINGLE;
                }

                node(std::wstring _re, std::wstring _value, NODETYPE t) {
                    re = _re;
                    regex = new wxRegEx(re.c_str(), wxRE_ADVANCED);
                    value = _value;
                    type = t;
                }

                virtual void print(int depth = 0) {
                    for (auto item : items) {
                        std::wstring padding(depth * 2, ' ');
                        std::cout << padding << (*item).re << std::endl;
                        if ((*item).value.length() > 0) {
                            std::cout << padding << "-> " << (*item).value << std::endl;
                        }
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

    public:


        node *insert(std::wstring key, VALUE value, NODETYPE type = SINGLE) {
            return root.insert(key, value, type);
        }

        void print() {
            root.print(0);
        }

        result scan(const wchar_t *str) {
            return result(&root, str);
        }

        node *get_root() {
            return &root;
        }

};


#endif
