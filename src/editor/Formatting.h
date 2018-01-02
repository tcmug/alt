#ifndef _EDITOR_FORMATTING_H_
#define _EDITOR_FORMATTING_H_

#include "../misc/Regtionary.h"
#include "Element.h"

#define IS_PRINTABLE(c) ((static_cast<unsigned char>(c) < 0x80) || (static_cast<unsigned char>(c) > 0xBF))

typedef Regtionary <Element*> Formatting;

#endif
