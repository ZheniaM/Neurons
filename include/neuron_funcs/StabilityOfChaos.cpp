#include "StabilityOfChaos.hpp"

#include "../SystemOf2NeuronsStateV2.hpp"
#include "../funcs.hpp"

std::unique_ptr<Points> StabilityOfChaos::get_points(void)
{
    auto points = std::make_unique<Points>();
    Point2x2 const StablePoint{P0};

    using EPS = std::remove_const_t<decltype(neps)>;
    for (EPS e = 0; e < neps; ++e)
    {
        eps = convertFromToLogy(e, 0, neps - 1, eps1, eps2);

        using PROCESS = std::remove_const_t<decltype(process)>;
        PROCESS out = 0;
        for (PROCESS p = 0; p < process ; ++p)
        {
            SystemOf2NeuronsStateV2 system{this};
            if ((system.next() - StablePoint).mod() > EPS_POINTS_EQUAL)
            {
                out++;
            }
        }

        points->add_point(eps, static_cast<double>(out) / static_cast<double>(process));
    }
    return points;
}