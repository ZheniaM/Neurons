#pragma once

#include <memory>
#include <TGraph.h>

#include "Points2.hpp"
#include "../Point2x2.hpp"

struct BifurcI
{
    Point2x2 const &P0;
    double const a;
    double const b;
    double const c;
    double const k;
    double const I1;
    double const I2;
    uint64_t const pass;
    uint64_t const process;
    uint64_t const nI;

    double I = 0.0; // для того чтобы можно было использовать конструктор SystemOf2NeuronState

    // ([]x1, []y1, []x2, []I)
    std::unique_ptr<Points2> get_points(void);
};

