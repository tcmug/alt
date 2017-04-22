#ifndef LIST_NODE_HPP
#define LIST_NODE_HPP

template <class T>
class list_node {

    public:

        list_node(): next(this), prev(this) {}
        list_node(T o): next(this), prev(this), owner(o) {}

        void insert(list_node *item) {
            item->remove();
            item->next = next;
            next->prev = item;
            item->prev = this;
            next = item;
        }

        list_node *remove() {
            list_node *ret = next;
            prev->next = next;
            next->prev = prev;
            prev = this;
            next = this;
            return ret;
        }

        list_node *get_next() {
            return next;
        }

        list_node *get_prev() {
            return prev;
        }

        T get_owner() {
            return owner;
        }

        list_node *begin() {
            return this;
        }

        list_node *end() {
            return this->prev;
        }

    private:

        T owner;
        list_node *next;
        list_node *prev;

};

#endif
