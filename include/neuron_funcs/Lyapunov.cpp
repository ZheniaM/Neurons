#include "Lyapunov.hpp"
#include "../SystemOf2NeuronsStateV2.hpp"
#include "../funcs.hpp"

double Lyapunov::exp()
{
    SystemOf2NeuronsStateV2 system(this);
    Point2x2 Pv = P0 + dP.normalized() * delta;
    double sumLogOfPi = 0;

    using PROCESS = std::remove_const_t<decltype(process)>;
    for (PROCESS i = 0; i < process; ++i)
    {
        SystemOf2NeuronsStateV2InitStateChaos sysInitState
        {
            .P0{Pv},
            .a{a},
            .b{b},
            .c{c},
            .I{I},
            .k{k},
            .eps{eps},
            .isStochastick{isStochastick},
        };
        SystemOf2NeuronsStateV2 systemv(&sysInitState);

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

    using K = std::remove_const_t<decltype(nk)>;
    for (K i = 0; i < nk; ++i)
    {
        k = convertFromTo(i, 0, nk - 1, k1, k2);
        double L = this->exp();
        points->add_point(k, L);
    }
    return points;
}