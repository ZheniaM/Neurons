#pragma once

#include "Points2.hpp"
#include "../Point2x2.hpp"
#include <stdint.h>
#include <memory>

struct SyncR2D
{
    Point2x2 const &P0;
    double const a;
    double const b;
    double const c;

    double const I1;
    double const I2;
    uint64_t const nI;

    double const k1;
    double const k2;
    uint64_t const nk;

    uint64_t const pass;
    uint64_t const process;

    // для того чтобы можно было использовать конструктор SystemOf2NeuronState
    double I = 0.0;
    double k = 0.0;

    std::unique_ptr<Points2> get_points(void);
};