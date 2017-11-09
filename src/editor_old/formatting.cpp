

#include "text_render_context.hpp"
#include "formatting.hpp"
#include "scan_context.hpp"

color::color() {
    _foreground = wxColour(255, 255, 255);
    _background = wxColour(0, 0, 0);
}

color::color(wxColour &fg, wxColour &bg): _foreground(fg), _background(bg) {
}

color::color(wxColour fg, wxColour bg): _foreground(fg), _background(bg) {
}


void color::print(scan_context *ctx) const {
    std::wstring str(ctx->res->getAt(), 0, ctx->res->getStart());

    const formatting::node *current = ctx->res->getCurrentNode();
    //const formatting::node *previous = ctx->res->get_previous_node();
    const formatting::node *parent = ctx->res->getParentNode();

    if (str.length() > 0) {
        //std::cout << (unsigned long)(this) << L": " << str << std::endl;
        _print(ctx, str);
    }

    // Snip should be printed with whatever is set in this node.
    if (ctx->res->getSnip().length() > 0) {
        //std::cout << (unsigned long)(this) << L": " << ctx->res->getSnip() << std::endl;
        ctx->tx->dc->SetTextBackground(_background);
        ctx->tx->dc->SetTextForeground(_foreground);
        _print(ctx, ctx->res->getSnip());
    }

    switch (current->getType()) {
        case formatting::SINGLE:
            ctx->tx->dc->SetTextBackground(parent->getValue()._background);
            ctx->tx->dc->SetTextForeground(parent->getValue()._foreground);
        break;
        case formatting::DROP:
            ctx->tx->dc->SetTextBackground(parent->getParent()->getValue()._background);
            ctx->tx->dc->SetTextForeground(parent->getParent()->getValue()._foreground);
        break;
        default:
        break;
    }

}


void color::_print(scan_context *ctx, const std::wstring &str) const {

    const std::wstring delim = L"\n";
    size_t from = 0, to = 0;

    while (std::wstring::npos != (to = str.find(delim, from))) {
        std::wstring part = str.substr(from, to - from);
        ctx->tx->print(part);

        ctx->line_states->push_back(line_state(ctx->tx->screen, wxSize(ctx->tx->position.x, ctx->tx->max_line_height), wxPoint(0, 0), 1, ctx->getNode()));

        ctx->tx->new_line();
        from = to + delim.length();
    }
    if (to - from > 0) {
        ctx->tx->print(str.substr(from, to - from));
    }
}


