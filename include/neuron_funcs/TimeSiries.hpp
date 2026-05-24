#pragma once

#include <memory>

#include "Points2.hpp"
#include "../Point2x2.hpp"

struct TimeSiries
{
    Point2x2 const &P0;
    double const a;
    double const b;
    double const c;
    double const k;
    double const I;
    uint64_t const pass;
    uint64_t const process;
    bool const isStochastick = false;
    double const eps = 0.0;


    // Time siries на отрезке от k1 до k2
    std::unique_ptr<Points2> get_points(void);
};