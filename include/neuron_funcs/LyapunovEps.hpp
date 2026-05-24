#pragma once

#include "Points2.hpp"
#include "../Point2x2.hpp"
#include <stdint.h>
#include <memory>

struct LyapunovEps
{
    Point2x2 P0;
    Point2x2 const &dP;
    double const a;
    double const b;
    double const c;

    double const I;
    double const k;

    // X-Axis
    double const eps1;
    // X-Axis
    double const eps2;
    // X-Axis
    uint64_t const neps;

    uint64_t const pass;
    uint64_t const process;

    // для того чтобы можно было использовать конструктор SystemOf2NeuronState
    double eps = 0.0;
    bool const isStochastick = true;

    std::unique_ptr<Points2> get_points(void);
};