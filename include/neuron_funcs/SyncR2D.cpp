#include "SyncR2D.hpp"

#include "../SystemOf2NeuronsStateV2.hpp"
#include "../funcs.hpp"

#include <TCanvas.h>
#include <TGraph.h>

std::unique_ptr<Points2> SyncR2D::get_points()
{
    auto points = std::make_unique<Points2>();
    points->reserve(nk * nI);

    for (std::remove_const_t<decltype(nk)> i = 0; i < nk; ++i)
    {
        k = convertFromTo(i, 0, nk - 1, k1, k2);
        for (std::remove_const_t<decltype(nI)> j = 0; j < nI; ++j)
        {
            I = convertFromTo(j, 0, nI - 1, I1, I2);
            SystemOf2NeuronsStateV2 system(this);
            // system.setIsStochastick(false);

            for (uint64_t i = 0; i < pass; ++i)
                system.next();

            std::vector<Point2x2> ps;
            ps.reserve(process);
            for (uint64_t i = 0; i < process; ++i)
            {
                Point2x2 p = system.next();
                ps.push_back(p);
            }

            double R = sync_measure_R(ps);
            (std::abs(R) > 1) && (R = 1);
            points->add_point(k, I, R, R);
        }
    }
    return points;
}