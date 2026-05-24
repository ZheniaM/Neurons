#pragma once

#include <memory>

#include "Points2.hpp"
#include "../Point2x2.hpp"

struct StochastickLevel
{
    Point2x2 const &P0;
    double const a;
    double const b;
    double const c;
    double const k;
    double const I;
    uint64_t const pass;
    uint64_t const process;
    double const mineps = 1e-8;
    double const maxeps = 1e-4;
    uint64_t const neps = 1'000;

    // не трогать

    bool const isStochastick = true;
    double eps;

    std::unique_ptr<Points2> get_points(void);
};