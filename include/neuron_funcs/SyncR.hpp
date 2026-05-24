#pragma once

#include <memory>

#include "Points.hpp"
#include "../Point2x2.hpp"

struct SyncR
{
    Point2x2 const &P0;
    double const a;
    double const b;
    double const c;
    double const k1;
    double const k2;
    double const I;
    uint64_t const pass;
    uint64_t const process;
    uint64_t const nk;

    double const eps = 0.0;
    bool const isStachastick = false;
    double k = 0.0; // для того чтобы можно было использовать конструктор SystemOf2NeuronState

    // Sync R на отрезке от k1 до k2
    std::unique_ptr<Points> get_points(void);
};