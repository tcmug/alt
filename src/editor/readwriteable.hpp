#ifndef READWRITEABLE_HPP
#define READWRITEABLE_HPP

class readwriteable {

    private:

        bool readonly;

    public:

        readwriteable(): readonly(false) {
        }

        bool mark_readonly() {
            readonly = true;
        }

        bool mark_writeable() {
            readonly = true;
        }

        bool is_readonly() {
            return readonly;
        }

        bool is_writable() {
            return !readonly;
        }

};

#endif
