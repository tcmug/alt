#ifndef _MISC_TOKENTREE_H_
#define _MISC_TOKENTREE_H_

#include <exception>
#include <map>

template <class CTYPE, class VALUE>
class TokenTree {


    public:

        TokenTree() {
            _root._type = ROOT;
        }

        void insert(CTYPE *key, int length, VALUE value) {

            Node *node = &_root;

            for (int si = 0; si < length; si++) {
                // Check if entry exists in map && use it ...
                auto i = node->_items.find(key[si]);
                if (i != node->_items.end()) {
                    node = i->second;
                    continue;
                }
                // ... create if not.
                Node *new_node = new Node();
                node->_items[key[si]] = new_node;
                node = new_node;
            }

            if (node->_type != NODE) {
               throw ExceptionDuplicate();
            }

            node->_depth = length;
            node->_value = value;
            node->_type  = LEAF;
        }

        VALUE match(CTYPE *at, CTYPE until, int *depth) {
            return _root.match(at, until, depth);
        }

        class ExceptionDuplicate: public std::exception {};
        class ExceptionNoMatch: public std::exception {};

    private:


        class Node;
        typedef std::map <CTYPE, Node*> DICTMAP;

        typedef enum {
            ROOT,
            NODE,
            LEAF
        } TYPE;

        class Node {
            public:
                Node(): _type(NODE) {
                }

                VALUE match(CTYPE *at, CTYPE until, int *depth) {
                    if (*at != until) {
                        auto i = _items.find(*at);

                        if (i == _items.end()) {
                            throw ExceptionNoMatch();
                        }

                        auto node = i->second;

                        if (node->_type == NODE ||
                            node->_items.size() > 0) {

                            // We're dealing with a NODE or a leaf that has kids.
                            try {
                                return node->match(at + 1, until, depth);
                            }
                            catch (ExceptionNoMatch &e) {
                            }
                        }

                        if (node->_type != NODE) {
                            // We're dealing with a proper match here
                            *depth = node->_depth;
                            return node->_value;
                        }
                    }
                    throw ExceptionNoMatch();
                }

                TYPE _type;
                DICTMAP _items;
                VALUE _value;
                int _depth;
        };

        Node _root;

};

#endif
