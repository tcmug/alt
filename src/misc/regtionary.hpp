#ifndef REGTIONARY_HPP
#define REGTIONARY_HPP

#include <wx/regex.h>
#include <string>
#include <vector>

template <class VALUE>
class regtionary {

    public:

        class node;

        enum NODETYPE {
            ROOT,
            SINGLE,
            ENTER,
            DROP
        };

        class result {

            public:

                result(node *cn, const wchar_t *at, const wchar_t *end):
                    _current_node(cn),
                    _previous_node(0),
                    _at(at),
                    _end(end),
                    _start(0),
                    _length(0),
                    _hits(0) {
                    if (!_end) {
                        _end = at + wcslen(at);
                    }
                }

                bool next() {

                    assert(_at != NULL);
                    assert(_current_node != NULL);

                    _previous_node = _current_node;

                    if (_current_node->_type == regtionary <VALUE>::SINGLE) {
                        _current_node = _current_node->_parent;
                    }
                    else if (_current_node->_type == regtionary <VALUE>::DROP) {
                        _current_node = _current_node->_parent->_parent;
                    }

                    _at = _at + (_start + _length);

                    if (_at >= _end || *_at == 0) {
                        return false;
                    }

                    _start = 100000;
                    _length = 0;
                    node *next_node = NULL;
                    int max_length = _end - _at;

                    for (auto item : _current_node->_items) {
                        if (item->_regex->Matches(_at)) {
                            size_t s, l;
                            (*item)._regex->GetMatch(&s, &l, 0);
                            // If this fails, replace <= with <
                            if (s < _start && s <= max_length && s <= max_length && (s + l) <= max_length) {
                                next_node = item;
                                _start = s;
                                _length = l;
                            }
                        }
                    }

                    // No match found, return full string as match.
                    if (next_node == NULL) {
                        _start = _end - _at;
                        _length = 0;
                        _snip = L"";
                        return true;
                    }

                    _snip = _at;
                    _snip = _snip.substr(_start, _length);

                    _current_node = next_node;

                    return true;
                }


                const node *getParentNode() const {
                    return _current_node->_parent;
                }

                const node *getCurrentNode() const {
                    return _current_node;
                }

                const node *getPreviousNode() const {
                    return _previous_node;
                }

                const std::wstring &getSnip() const {
                    return _snip;
                }

                const wchar_t *getAt() const {
                    return _at;
                }

                size_t getStart() const {
                    return _start;
                }

            private:

                node *_current_node;
                node *_previous_node;

                const wchar_t *_at;
                const wchar_t *_end;

                std::wstring _snip;
                size_t _start, _length;
                int _hits;

        };

        class node {

            friend class result;

            private:

                VALUE _value;
                NODETYPE _type;

                std::wstring _re;
                std::vector <node*> _items;
                wxRegEx *_regex;
                node *_parent;

                node(std::wstring re, VALUE value, NODETYPE t) {
                    _re = re;
                    _regex = new wxRegEx(_re.c_str(), wxRE_ADVANCED);
                    _value = value;
                    _type = t;
                }

            public:

                node():  _type(ROOT), _parent(NULL) {
                }

                virtual void print(int depth = 0) {
                    for (auto item : _items) {
                        std::wstring padding(depth * 2, ' ');
                        std::cout << padding << (*item)._re << std::endl;
                        if ((*item)._items.size() > 0) {
                            (*item).print(depth + 1 );
                        }
                    }
                }

                virtual bool last() {
                    return _items.size() == 0;
                }

                node *insert(std::wstring key, VALUE value, NODETYPE type = SINGLE) {
                    node *nn = new node(key, value, type);
                    nn->_parent = this;
                    _items.push_back(nn);
                    return nn;
                }

                NODETYPE getType() const {
                    return _type;
                }

                const node *getParent() const {
                    return _parent;
                }

                VALUE getValue() const {
                    return _value;
                }

        };

        node *insert(std::wstring key, VALUE value, NODETYPE type = SINGLE) {
            return root.insert(key, value, type);
        }

        void print() {
            root.print(0);
        }

        result scan(const wchar_t *str, const wchar_t *end = 0) {
            return result(&root, str, end);
        }

        node *getRoot() {
            return &root;
        }

    private:

        node root;


};


#endif
