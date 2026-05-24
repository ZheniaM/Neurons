#include "Bifurc.hpp"
#include "../funcs.hpp"
#include "../SystemOf2NeuronsStateV2.hpp"


std::unique_ptr<Points2> Bifurc::get_points(void) 
{
    auto points = std::make_unique<Points2>();
    points->reserve(nk * process);
    
    for (uint64_t s = 0; s < nk; ++s)
    {
        k = convertFromTo(s, 0, nk - 1, k1, k2);
        SystemOf2NeuronsStateV2 system(this);
        // system.setIsStochastick(false);

        for (uint64_t i = 0; i < pass; ++i)
        {
            Point2x2 p = system.next();
        }

        for (uint64_t i = 0; i < process; ++i)
        {
            Point2x2 p = system.next();
            // points->add_point(k, p.get_x1(), k, p.get_x2());
            points->add_point(k, p.get_x1(), k, p.get_y1());
        }
    }
    
    return points;
}