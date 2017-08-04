#ifndef TEXT_RENDER_CONTEXT_HPP
#define TEXT_RENDER_CONTEXT_HPP

#include "wx/wx.h"
#include "state.hpp"
#include "../misc/regtionary.hpp"

class text_render_context;
class scan_context;


class color {
    public:

        color() {
            foreground = wxColour(255, 255, 255);
            background = wxColour(0, 0, 0);
        }

        color(wxColour &fg, wxColour &bg): foreground(fg), background(bg) {
        }

        color(wxColour fg, wxColour bg): foreground(fg), background(bg) {
        }

        void print(scan_context *tx);

        wxColour foreground;
        wxColour background;

    private:

        void _print(scan_context *ctx, const std::wstring &str);
};

typedef regtionary <color> formatting;


class line_state {

    public:

        line_state(wxPoint _screen, wxSize _extents, formatting::node *_node): screen(_screen), extents(_extents), current_node(_node) {
        }

        wxPoint screen;
        wxSize extents;
        formatting::node *current_node;
};


class text_render_context {
    public:

        text_render_context(wxDC *_dc, formatting *format);
        void print(const std::wstring &content);
        wxSize get_extents(const std::wstring &content);
        void new_line();

        wxDC *dc;

        wxPoint position;
        wxPoint screen;

        wxPoint viewport_position;
        wxSize viewport_extents;

        int max_line_width;
        int max_line_height;

        formatting *format;

        void report();
        void reset();
};

class scan_context {
    public:
        scan_context(std::vector <line_state> *_line_states, formatting::result *_res, text_render_context *_tx): line_states(_line_states), res(_res), tx(_tx) {
        }

        std::vector <line_state> *line_states;
        formatting::result *res;
        text_render_context *tx;
};

#endif
