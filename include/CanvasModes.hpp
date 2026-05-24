#pragma once

#include <stdint.h>

#define CANVAS_MODES                        \
    CANVAS_MODES_X(one, "One", 1, (1, 1))              \
    CANVAS_MODES_X(topBotom, "Top & Botom", 2, (2, 1)) \
    CANVAS_MODES_X(four, "Four", 4, (2, 2))

namespace CanvasModes
{
#define CANVAS_MODES_X(mode, txt, max, dividerule) mode,
    enum id : int32_t
    {
        CANVAS_MODES
    };
#undef CANVAS_MODES_X
} 
