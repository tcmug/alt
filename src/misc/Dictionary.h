#ifndef _MISC_DICTIONARY_HPP_
#define _MISC_DICTIONARY_HPP_

#include "TokenTree.h"

#include <exception>
#include <iostream>
#include <string.h>

template <class VALUE>
class Dictionary {

    public:

        class Node;

        enum NODETYPE {
            ROOT,
            SINGLE,
            ENTER,
            DROP,
            NODE
        };

        class Result {

            public:

                Result(Node *cn, const char *at, const char *end):
                    _currentNode(cn),
                    _previousNode(0),
                    _nextNode(0),
                    _rootNode(cn),
                    _prevEnd(0),
                    _at(at),
                    _end(end),
                    _length(0) {
                    if (!_end) {
                        _end = at + strlen(at);
                    }
                }

                Result(): _currentNode(0),
                    _previousNode(0),
                    _nextNode(0),
                    _rootNode(0),
                    _prevEnd(0),
                    _at(0),
                    _end(0),
                    _length(0) {
                }

                bool next() {

                    _at += _length;

                    if (_nextNode) {
                        _currentNode = _nextNode;
                        _nextNode = 0;
                        _length = _matchLength;
                        _matchLength = 0;
                        return true;
                    }
                    else {
                        _currentNode = _rootNode;
                    }

                    _cursor = _at;
                    _matchLength = 0;

                    while (_matchLength == 0 && _cursor < _end) {
                        _nextNode = _currentNode->match(_cursor, &_matchLength);
                        if (_matchLength == 0) {
                            _cursor++;
                        }
                    }

                    _length = _cursor - _at;
                    _cursor += _length;

                    // std::cout << "processing: " << _length << "/" << _matchLength << " " << _at << " @" << (unsigned long)(_currentNode) << std::endl;

                    return _at < _end;
                }

                const Node *getParentNode() const {
                    return _currentNode->_parent;
                }

                const Node *getCurrentNode() const {
                    return _currentNode;
                }

                const Node *getPreviousNode() const {
                    return _previousNode;
                }

                const char *getAt() const {
                    return _at;
                }

                size_t getLength() const {
                    return _length;
                }

            private:

                Node *_currentNode;
                Node *_previousNode;
                Node *_nextNode;
                Node *_rootNode;

                const char *_at;
                const char *_cursor;
                const char *_prevEnd;
                const char *_end;

                size_t _length;
                int _matchLength;

        };

        class Node {

            friend class Result;

            private:

                VALUE _value;
                NODETYPE _type;

                TokenTree <const char, Node*> _tree;
                Node *_parent;

                Node(VALUE value, NODETYPE t) {
                    _value = value;
                    _type = t;
                }

            public:

                Node(NODETYPE type): _type(type) {
                }

                Node(): _type(ROOT), _parent(NULL) {
                }

                Node *match(const char *at, int *depth) {
                    try {
                       return _tree.match(at, 0, depth);
                    }
                    catch (std::exception &e) {
                        return 0;
                    }
                }

                Node *insert(std::string key, VALUE value, NODETYPE type = SINGLE) {
                    Node *node = new Node();
                    node->_value = value;
                    _tree.insert(key.c_str(), key.length(), node);
                    return node;
                }


                NODETYPE getType() const {
                    return _type;
                }

                const Node *getParent() const {
                    return _parent;
                }

                VALUE getValue() const {
                    return _value;
                }

                void setValue(VALUE newValue) {
                    _value = newValue;
                }

        };

        Node *insert(std::string key, VALUE value, NODETYPE type = SINGLE) {
            return root.insert(key, value, type);
        }

        Result scan(const char *str, const char *end = 0) {
            return Result(&root, str, end);
        }

        Node *getRoot() {
            return &root;
        }

    private:

        Node root;


};


#endif
