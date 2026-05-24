#pragma once

#include "Points.hpp"
#include "../Point2x2.hpp"
#include <stdint.h>
#include <memory>

struct Snapshot
{
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
    uint64_t const t;

    std::unique_ptr<Points> get_points(void);
};
