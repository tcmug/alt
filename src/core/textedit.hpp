#ifndef TEXTEDIT_HPP
#define TEXTEDIT_HPP

#include "wx/wx.h"




class text_render_context {
    public:

        text_render_context(wxDC *_dc);
        void print(std::wstring content);

        wxDC *dc;

        wxPoint position;
        wxPoint screen;

        int max_line_width;
        int max_line_height;
        int left_padding;
};



class text_caret {
    public:

        text_caret(wxPoint _position, wxPoint _screen);
        void render(text_render_context &tx) const;

        wxPoint screen;
        wxPoint position;

        bool operator < (const text_caret& other) const {
            if (position.y == other.position.y)
                return (position.x > other.position.x);
            return (position.y > other.position.y);
        }

        void report() {
            std::cout << position.x << " " << position.y << std::endl;
        }

};


class text_line {
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

        void render(text_render_context &tx) const;
        int get_length() const;

        std::wstring cut(int pos);

        void insert(int pos, std::wstring str);
        void erase(int pos);
};


#endif
