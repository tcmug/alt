#ifndef TEXT_LINE_HPP
#define TEXT_LINE_HPP

#include "wx/wx.h"

class text_caret;

#include "text_render_context.hpp"
#include "dirtyable.hpp"
#include "readwriteable.hpp"

class text_line:
    public dirtyable,
    public readwriteable {

    private:

        bool render_text(text_render_context &tx) const;
        void update_specs();

        int changed;
        std::wstring content;
        wxSize extents;

    public:

        text_line(std::wstring str);

        int map_point_to_column(text_render_context &dc, int x) const;
        int map_column_to_point(text_render_context &dc, int x) const;

        void update(text_render_context &tx);
        void render(text_render_context &tx) const;
        int get_length() const;

        std::wstring cut(int pos);

        void insert(int pos, std::wstring str);
        void erase(int pos);

        int get_line_height() const;
};


#endif
