#ifndef DIRTYABLE_HPP
#define DIRTYABLE_HPP

class dirtyable {

    private:

        bool dirty;

    public:

        dirtyable(): dirty(true) {
        }

        void mark_dirty() {
            dirty = true;
        }

        void mark_clean() {
            dirty = false;
        }

        bool is_dirty() {
            return dirty;
        }

        bool is_clean() {
            return !dirty;
        }

};

#endif
