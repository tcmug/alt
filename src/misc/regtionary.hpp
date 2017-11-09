#ifndef REGTIONARY_HPP
#define REGTIONARY_HPP

#include <regex>
#include <string>
#include <vector>
#include <assert.h>

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

                result(node *cn, const char *at, const char *end):
                    _current_node(cn),
                    _previous_node(0),
                    _at(at),
                    _end(end),
                    _length(0),
                    _next_node(0) {
                    if (!_end) {
                        _end = at + strlen(at);
                    }
                }

                bool next() {

                    assert(_at != NULL);
                    assert(_current_node != NULL);

                    // SPECIAL CASE: Already matched a node.
                    if (_next_node) {
                    // FIRST: from _at until _at + _length with current_node
                    // SECOND: from _at + _length until _at + _length + _match_length
                        _at += _length;
                        _length = _match_length;
                        _current_node = _next_node;
                        _next_node = 0;
                        return true;
                    }

                    _at += _length;

                    _previous_node = _current_node;

                    if (_current_node->_type == regtionary <VALUE>::SINGLE) {
                        _current_node = _current_node->_parent;
                    }
                    else if (_current_node->_type == regtionary <VALUE>::DROP) {
                        _current_node = _current_node->_parent->_parent;
                    }

                    // END.
                    if (_at >= _end || *_at == 0) {
                        return false;
                    }

                    size_t min_s = 100000;
                    _length = 0;

                    size_t max_length = _end - _at;
                    _next_node = NULL;

                    for (auto item : _current_node->_items) {
                        std::smatch match;
                        if (std::regex_search(std::string(_at), match, (*(*item)._regex))) {
                            size_t s = match.position(), l = match.length();
                            if (s < min_s && s <= max_length && s <= max_length && (s + l) <= max_length) {
                                _next_node = item;
                                _length = s;
                                _match_length = l;
                                min_s = s;
                            }
                        }
                    }

                    // from _at until _at + _start with current_node
                    // from _at + _start until _at + _start + _length with next_node

                    // No match found, return full string as match.
                    if (_next_node == NULL) {
                        _length = _end - _at;
                        return true;
                    }

                    if (_length == 0) {
                        _current_node = _next_node;
                        _next_node = 0;
                        _length = _match_length;
                    }

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

                // const std::string &getSnip() const {
                //     return _snip;
                // }

                const char *getAt() const {
                    return _at;
                }

                size_t getLength() const {
                    return _length;
                }

            private:

                node *_current_node;
                node *_previous_node;
                node *_next_node;

                const char *_at;
                const char *_end;

                size_t _length, _match_length;

        };

        class node {

            friend class result;

            private:

                VALUE _value;
                NODETYPE _type;

                std::string _re;
                std::vector <node*> _items;
                std::regex *_regex;
                node *_parent;

                node(std::string re, VALUE value, NODETYPE t) {
                    _re = re;
                    _regex = new std::regex(_re);
                    //_regex = new wxRegEx(_re.c_str(), wxRE_ADVANCED);
                    _value = value;
                    _type = t;
                }

            public:

                node():  _type(ROOT), _parent(NULL) {
                }

                // virtual void print(int depth = 0) {
                //     for (auto item : _items) {
                //         std::string padding(depth * 2, ' ');
                //         std::cout << padding << (*item)._re << std::endl;
                //         if ((*item)._items.size() > 0) {
                //             (*item).print(depth + 1 );
                //         }
                //     }
                // }

                virtual bool last() {
                    return _items.size() == 0;
                }

                node *insert(std::string key, VALUE value, NODETYPE type = SINGLE) {
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

        node *insert(std::string key, VALUE value, NODETYPE type = SINGLE) {
            return root.insert(key, value, type);
        }

        void print() {
            root.print(0);
        }

        result scan(const char *str, const char *end = 0) {
            return result(&root, str, end);
        }

        node *getRoot() {
            return &root;
        }

    private:

        node root;


};


#endif
