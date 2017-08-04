
#include "text_render_context.hpp"

#include <algorithm>
#include <iostream>
#include <vector>


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


text_render_context::text_render_context(wxDC *_dc, formatting *frmt):
    dc(_dc),
    screen(0, 0),
    position(1, 1),
    max_line_width(0),
    max_line_height(0),
    format(frmt) {
}


void text_render_context::reset() {
    screen.x = 0;
    screen.y = 0;
    position.x = 1;
    position.y = 1;
}


template <typename T>
std::vector <T> split_string(const T string, T delim) {
    std::vector <T> result;
    size_t from = 0, to = 0;
    while (T::npos != (to = string.find(delim, from))) {
        result.push_back(string.substr(from, to - from));
        from = to + delim.length();
    }
    result.push_back(string.substr(from, to));
    return result;
}



void text_render_context::print(const std::wstring &content) {
    wxSize sz = get_extents(content);
    assert(content.find(L"\n") == std::wstring::npos);
    dc->DrawText(content, screen.x, screen.y);
    max_line_height = std::max(max_line_height, sz.GetHeight());
    screen.x += sz.GetWidth();
}


wxSize text_render_context::get_extents(const std::wstring &content) {
    if (content == L"") {
        wxSize temp = dc->GetTextExtent(L" ");
        temp.SetWidth(0);
        return temp;
    }
    return dc->GetTextExtent(content);
}


void text_render_context::report() {
    std::cout << "Location: ";
    std::cout << position.x << " ";
    std::cout << position.y << " Screen: ";
    std::cout << screen.x << " ";
    std::cout << screen.y << " ";
    std::cout << std::endl;
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


void text_render_context::new_line() {

    // line_state(wxPoint _screen, wxSize _extents, formatting::node *_node): screen(_screen), extents(_extents), current_node(_node) {

    position.y++;
    position.x = 0;
    screen.y += max_line_height;
    max_line_height = 0;
    screen.x = 0;
}
