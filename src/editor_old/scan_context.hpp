#ifndef SCAN_CONTEXT_HPP
#define SCAN_CONTEXT_HPP

#include "wx/wx.h"
#include "../misc/regtionary.hpp"
#include "formatting.hpp"

class text_render_context;
class scan_context;


class line_state {

    public:

        line_state(wxPoint screen, wxSize extents, wxPoint caret, std::size_t position, const formatting::node *node):
            _screen(screen),
            _extents(extents),
            _caret(caret),
            _current_node(node) {
        }

        const wxPoint &getScreen() const {
            return _screen;
        }

        const wxSize &getExtents() const {
            return _extents;
        }

        const wxPoint &getCaret() const {
            return _caret;
        };

        const std::size_t &getPosition() const {
            return _position;
        };

        const formatting::node *getNode() const {
            return _current_node;
        };

        void report() {
            std::wcout << _screen.x << L"," << _screen.y << L" -- ";
            std::wcout << _extents.GetWidth() << L"," << _extents.GetHeight() << L" -- ";
            std::wcout << _caret.x << L"," << _caret.y << L" -- ";
            std::wcout << std::endl;
        }

    private:

        wxPoint _screen;
        wxSize _extents;
        wxPoint _caret;
        std::size_t _position;
        const formatting::node *_current_node;
};


class scan_context {
    public:
        scan_context(std::vector <line_state> *_line_states, formatting::result *_res, text_render_context *_tx): line_states(_line_states), res(_res), tx(_tx) {
        }

        const formatting::node *getNode() const {
            return res->getCurrentNode();
        };

        std::vector <line_state> *line_states;
        formatting::result *res;
        text_render_context *tx;
};

#endif
