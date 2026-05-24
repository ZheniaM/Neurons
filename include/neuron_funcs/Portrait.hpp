#pragma once

#include "Points2.hpp"
#include "../Point2x2.hpp"
#include <stdint.h>
#include <memory>

struct Portrait
{
    Point2x2 const &P0;
    double const a;
    double const b;
    double const c;
    double const I;
    double const k;
    uint64_t const pass;
    uint64_t const process;
    bool const isStochastick = false;
    double const eps = 0.0;

    std::unique_ptr<Points2> get_points(void);
};