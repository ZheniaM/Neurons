#include "SyncR2DChaos.hpp"
#include "SyncR.hpp"

#include "../funcs.hpp"
#include "../SystemOf2NeuronsStateV2.hpp"

std::unique_ptr<Points2> SyncR2DChaos::get_points(void)
{
    auto points = std::make_unique<Points2>();
    points->reserve(nk * neps);
    using K = std::remove_const_t<decltype(nk)>;
    using EPS = std::remove_const_t<decltype(neps)>;

    {
        using PASS = std::remove_const_t<decltype(pass)>;
        SystemOf2NeuronsStateV2 s{this};
        for (PASS p = 0; p < pass; ++p)
        {
            P0 = s.next();
        }
    }
    printPoint2x2(P0);

    for (EPS _eps = 0; _eps < neps; ++_eps)
    {
        eps = convertFromTo(_eps, 0, neps - 1, eps1, eps2);

        SyncR syncR{
            .P0{P0},
            .a{a},
            .b{b},
            .c{c},
            .k1{k1},
            .k2{k2},
            .I{I},
            .pass{pass},
            .process{process},
            .nk{nk},
            .eps{eps},
            .isStachastick{isStochastick},
        };

        auto ps = syncR.get_points(); // {k[], R[]}
        using SIZE = std::remove_const_t<decltype(ps->size())>;
        for (SIZE s = 0; s < ps->size(); ++s)
        {
            points->add_point(ps->get_x_at(s), eps, ps->get_y_at(s), ps->get_y_at(s));
        }
    }
    return points;
}
