#include "BifurcI.hpp"
#include "../funcs.hpp"
#include "../SystemOf2NeuronsStateV2.hpp"

std::unique_ptr<Points2> BifurcI::get_points(void)
{
    using S = std::remove_const_t<decltype(nI)>;
    using PASS = std::remove_const_t<decltype(pass)>;
    using PROCESS = std::remove_const_t<decltype(process)>;

    auto points = std::make_unique<Points2>();
    points->reserve(nI * process);
    for (S s = 0; s < nI; ++s)
    {
        I = convertFromTo(s, 0, nI - 1, I1, I2);
        SystemOf2NeuronsStateV2 system(this);
        // system.setIsStochastick(false);

        for (PASS i = 0; i < pass; ++i)
        {
            Point2x2 p = system.next();
        }

        for (PROCESS i = 0; i < process; ++i)
        {
            Point2x2 p = system.next();
            points->add_point(p.get_x1(), p.get_y1(), p.get_x2(), I);
        }
    }

    return points;
}