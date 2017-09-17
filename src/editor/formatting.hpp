#ifndef FORMATTING_HPP
#define FORMATTING_HPP

#include "wx/wx.h"
#include "../misc/regtionary.hpp"

class text_render_context;
class scan_context;

class color {
    public:

        color();
        color(wxColour &fg, wxColour &bg);
        color(wxColour fg, wxColour bg);

        void print(scan_context *tx) const;

    private:

        wxColour _foreground;
        wxColour _background;

        void _print(scan_context *ctx, const std::wstring &str) const;
};

typedef regtionary <color> formatting;

#endif
