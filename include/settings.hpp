#pragma once
#include "Point2x2.hpp"
#include <stdint.h>
#include <TColor.h>

// #define SPAIN_VARIANT

namespace settings
{

    // bif
    extern double const BIF_RANGE_START, BIF_RANGE_END;

    // time siries
    extern uint64_t const TS_FROM, TS_TO;

    extern double const I, a, b, c, sigma;
    extern bool const isStachastick;
    extern double const EPS;
    extern Point2x2 const P0;

    namespace graphic
    {
        extern Style_t const MARKER_STYLE;
        extern Size_t const MARKER_SIZE;
        extern Width_t const LINE_WIDTH;
        extern EColor const X1_COLOR, X2_COLOR;
        extern uint64_t const CANVAS_WIDTH, CANVAS_HEIGHT;
    }

    namespace portrait
    {
        extern double const I;
        extern int64_t const sigma1, sigma2, ratio, inc;
        extern uint64_t const pass, draw;
        extern EColor const color, colorR;
        extern bool const drawMPortraits;
    }
}
