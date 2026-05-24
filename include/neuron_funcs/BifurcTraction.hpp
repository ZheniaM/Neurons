#pragma once

#include <memory>


#include "Points2.hpp"
#include "../Point2x2.hpp"

struct BifurcTraction// : IPoints2Getter
{
    Point2x2 const &P0;
    double const a;
    double const b;
    double const c;
    double const k1;
    double const k2;
    double const kStart;
    double const I;
    uint64_t const pass;
    uint64_t const process;
    uint64_t const nk;

    double k = 0.0; // для того чтобы можно было использовать конструктор SystemOf2NeuronState

    // Дает точки биффуркации в k-x1, k-x2 пространствах
    std::unique_ptr<Points2> get_points(void);
};