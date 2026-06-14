#pragma once

#include "Points2.hpp"
#include "../Point2x2.hpp"
#include <stdint.h>
#include <memory>

struct TransientBasin
{
    Point2x2 const PStable;
    Point2x2 P0;
    double const a;
    double const b;
    double const c;
    double const I;
    double const k;

    double const minx1;
    double const miny1;
    double const maxx1;
    double const maxy1;
    
    uint64_t const nx1;
    uint64_t const ny1;
    uint64_t const maxt;
    double const delta = 1e-4;

    std::unique_ptr<Points2> get_points(void);
};
