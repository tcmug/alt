

#include "text_render_context.hpp"
#include "formatting.hpp"

color::color() {
    foreground = wxColour(255, 255, 255);
    background = wxColour(0, 0, 0);
}

color::color(wxColour &fg, wxColour &bg): foreground(fg), background(bg) {
}

color::color(wxColour fg, wxColour bg): foreground(fg), background(bg) {
}


void color::print(scan_context *ctx) {
    std::wstring str(ctx->res->at, 0, ctx->res->start);

    const formatting::node *current = ctx->res->current_node;
    const formatting::node *parent = ctx->res->current_node->parent;

    if (str.length() > 0) {
        _print(ctx, str);
    }

    // Snip should be printed with whatever is set in this node.
    if (ctx->res->snip.length() > 0) {
        ctx->tx->dc->SetTextBackground(background);
        ctx->tx->dc->SetTextForeground(foreground);
        _print(ctx, ctx->res->snip);
    }

    switch (current->type) {
        case formatting::SINGLE:
            ctx->tx->dc->SetTextBackground(parent->value.background);
            ctx->tx->dc->SetTextForeground(parent->value.foreground);
        break;
        case formatting::DROP:
            ctx->tx->dc->SetTextBackground(parent->parent->value.background);
            ctx->tx->dc->SetTextForeground(parent->parent->value.foreground);
        break;
    }

}


void color::_print(scan_context *ctx, const std::wstring &str) {

    const std::wstring delim = L"\n";
    size_t from = 0, to = 0;
    int x = 0;

    while (std::wstring::npos != (to = str.find(delim, from))) {
        std::wstring part = str.substr(from, to - from);
        ctx->tx->print(part);

        ctx->line_states->push_back(line_state(ctx->tx->screen, wxSize(ctx->tx->position.x, ctx->tx->max_line_height), 0));

        ctx->tx->new_line();
        from = to + delim.length();
    }
    if (to - from > 0) {
        ctx->tx->print(str.substr(from, to - from));
    }
}


