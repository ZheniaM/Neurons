#include "StochastickLevel.hpp"

#include "../funcs.hpp"
#include "../SystemOf2NeuronsStateV2.hpp"

std::unique_ptr<Points2> StochastickLevel::get_points(void)
{
    auto points = std::make_unique<Points2>();

    for (uint64_t e = 0; e < neps; ++e)
    {
        eps = convertFromTo(e, 0, neps - 1, mineps, maxeps);
        SystemOf2NeuronsStateV2 system(this);

        for (uint64_t i = 0; i < pass; ++i)
        {
            system.next();
        }

        for (uint64_t i = 0; i < process; ++i)
        {
            Point2x2 p = system.next();
            points->add_point(eps, p.get_x1(), eps, p.get_y1());
        }
    }

    return points;
}