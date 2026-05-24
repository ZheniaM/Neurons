#pragma once

#include <memory>

#include "Points.hpp"
#include "../Point2x2.hpp"

struct StabilityOfChaos
{
    Point2x2 const &P0; // STABLE POINT
    double const a;
    double const b;
    double const c;
    double const I;
    double const k;
    
    double const eps1;
    double const eps2;
    uint64_t const neps;
    uint64_t const process;


    double eps = 0.0;
    double const EPS_POINTS_EQUAL = 1e-4;
    bool const isStochastick = true;


    // Sync R на отрезке от k1 до k2
    std::unique_ptr<Points> get_points(void); // [[eps, #stable]]
};