#include "SyncR.hpp"

#include <stdint.h>

#include "../SystemOf2NeuronsStateV2.hpp"
#include "../funcs.hpp"

#include <TCanvas.h>
#include <TGraph.h>

std::unique_ptr<Points> SyncR::get_points()
{
    auto points = std::make_unique<Points>();
    points->reserve(nk);

    if (k1 == k2)
    {
        SystemOf2NeuronsStateV2 system(this);
        for (uint64_t i = 0; i < pass; ++i)
        {
            system.next();
        }

        std::vector<Point2x2> ps;
        ps.reserve(process);
        for (uint64_t i = 0; i < process; ++i)
        {
            ps.push_back(system.next());
        }
        double R = sync_measure_R(ps);
        points->add_point(k1, R);
        printf("k = %lf, R(X) = %lf\n", k1, R);
        return points;
    }

    for (uint64_t i = 0; i < nk; ++i)
    {
        k = convertFromTo(i, 0, nk - 1, k1, k2);
        SystemOf2NeuronsStateV2 system(this);

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
        points->add_point(k, R);
    }
    return points;
}