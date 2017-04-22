#ifndef STATE_HPP
#define STATE_HPP

#include <stack>
#include <assert.h>

template <class I>
class state_stack;

template <class I>
class state {

    protected:
        typedef state_stack <I> state_stack_type;

    public:

        state() {
        }

        virtual ~state() {
        }

        virtual void exit(state_stack_type *s) {
        }

        virtual void enter(state_stack_type *s) {
        }

        virtual void handle_input(state_stack_type *s, I input) = 0;

};

template <class I>
class state_stack {

    protected:
        typedef state <I> state_item;

    private:

        std::stack <state_item*> stack_items;
        state_item *root;
        bool modified;

    public:

        state_stack(state_item *_root) {
            stack_items.push(_root);
            root = _root;
            modified = false;
        }

        void push(state_item *item) {
            item->enter(this);
            stack_items.push(item);
            modified = true;
        }

        void reset() {
            while (!stack_items.empty()) {
                stack_items.pop();
            }
            stack_items.push(root);
            modified = false;
        }

        state_item *pop() {
            assert(stack_items.size() > 1);
            state_item *item = stack_items.top();
            item->exit(this);
            stack_items.pop();
            modified = true;
            return item;
        }

        void handle_input(I input) {
            do {
                modified = false;
                stack_items.top()->handle_input(this, input);
            } while (modified);

        }

        const state_item *current() const {
            return stack_items.top();
        }

};


#endif
