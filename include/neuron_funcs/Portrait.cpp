#include "Portrait.hpp"

#include "../SystemOf2NeuronsStateV2.hpp"

std::unique_ptr<Points2> Portrait::get_points(void)
{
    auto points = std::make_unique<Points2>();
    // SystemOf2NeuronsStateV2 system(P0, k, I, a, abc[1], abc[2]);
    SystemOf2NeuronsStateV2 system(this);
    // system.setIsStochastick(false);
    
    for (int32_t i = 0; i < pass; ++i)
    {
        system.next();
    }

    points->reserve(process);
    for (int32_t i = 0; i < process; ++i)
    {
        Point2x2 p = system.next();
        points->add_point(p);
    }
    return points;
}