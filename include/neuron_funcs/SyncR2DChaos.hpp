#pragma once

#include "Points2.hpp"
#include "../Point2x2.hpp"
#include <stdint.h>
#include <memory>

struct SyncR2DChaos
{
    Point2x2 P0;
    double const a;
    double const b;
    double const c;

    double const I;

    // X-Axis
    double const k1;
    // X-Axis
    double const k2;
    // X-Axis
    uint64_t const nk;

    // Y-Axis
    double const eps1;
    // Y-Axis
    double const eps2;
    // Y-Axis
    uint64_t const neps;

    uint64_t const pass;
    uint64_t const process;

    // для того чтобы можно было использовать конструктор SystemOf2NeuronState
    double k = 0.0;
    double eps = 0.0;

    bool const isStochastick = true;

    std::unique_ptr<Points2> get_points(void);
};