#pragma once

#include <memory>

#include "Points.hpp"
#include "../Point2x2.hpp"

struct Lyapunov
{
    Point2x2 const &P0;
    Point2x2 const &dP;
    double const a;
    double const b;
    double const c;
    double const I;
    double const k1;
    double const k2;
    // uint64_t const pass;
    uint64_t const process;
    uint64_t const nk;

    double const eps = 0.0;
    bool const isStochastick = false;

    double const delta = 1e-6;

    double k = 0.0;

    double exp(void);
    // Time siries на отрезке от k1 до k2
    std::unique_ptr<Points> get_points(void);
};