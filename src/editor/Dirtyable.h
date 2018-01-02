#ifndef _EDITOR_DIRTYABLE_H_
#define _EDITOR_DIRTYABLE_H_

class Dirtyable {

    private:

        bool _dirty;

    public:

        Dirtyable(): _dirty(true) {
        }

        void markDirty() {
            _dirty = true;
        }

        void markClean() {
            _dirty = false;
        }

        bool isDirty() {
            return _dirty;
        }

        bool isClean() {
            return !_dirty;
        }

};

#endif
