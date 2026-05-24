#include "Lyapunov.hpp"
#include "../SystemOf2NeuronsStateV2.hpp"
#include "../funcs.hpp"

// #include <optional>
#include <stdint.h>

struct V
{
    double a, b, c;
    double I, k;
    Point2x2 P0;
    double eps = 0.0;
    bool isStochastick = false;
};

double Lyapunov::exp()
{
    SystemOf2NeuronsStateV2 system(this);
    Point2x2 Pv = P0 + dP.normalized() * delta;
    double sumLogOfPi = 0;

    for (uint64_t i = 0; i < process; ++i)
    {
        V v{
            .a{a},
            .b{b},
            .c{c},
            .I{I},
            .k{k},
            .P0{Pv},
            .eps{eps},
            .isStochastick{isStochastick},
        };
        SystemOf2NeuronsStateV2 systemv(&v);

        Point2x2 Pnext = system.next();
        Point2x2 Pbar = systemv.next();
        Point2x2 dPbarPnext = Pbar - Pnext;
        double mod = dPbarPnext.mod();
        double p = mod / delta;
        sumLogOfPi += std::log(p);

        Pv = Pnext + dPbarPnext * (delta / mod);
    }

    return sumLogOfPi / process;
}

std::unique_ptr<Points> Lyapunov::get_points(void)
{
    auto points = std::make_unique<Points>();
    points->reserve(nk);

    for (int64_t i = 0; i < nk; ++i)
    {
        k = convertFromTo(i, 0, nk - 1, k1, k2);
        double L = this->exp();
        points->add_point(k, L);
    }
    return points;
}