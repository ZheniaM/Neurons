#include "BifurcTraction.hpp"

#include "../funcs.hpp"
#include "../SystemOf2NeuronsStateV2.hpp"

std::unique_ptr<Points2> BifurcTraction::get_points()
{
    auto points = std::make_unique<Points2>();
    points->reserve(static_cast<std::size_t>(nk) * process);

    int64_t iStart = convertFromTo(kStart, k1, k2, 0, nk - 1);

    Point2x2 pRight(P0);
    for (int64_t i = iStart; i < nk; ++i)
    {
        k = convertFromTo(i, 0, nk - 1, k1, k2);
        SystemOf2NeuronsStateV2 system(this);
        // system.setIsStochastick(false);

        for (uint64_t i = 0; i < pass; ++i)
        {
            system.next();
        }

        for (uint64_t i = 0; i < process; ++i)
        {
            pRight = system.next();
            points->add_point(k, pRight.get_x1(), k, pRight.get_x2());
        }
    }

    Point2x2 pLeft(P0);
    for (int64_t i = iStart; 0 < i; --i)
    {
        k = convertFromTo(i, 0, nk - 1, k1, k2);
        SystemOf2NeuronsStateV2 system(this);
        // system.setIsStochastick(false);

        for (uint64_t i = 0; i < pass; ++i)
        {
            system.next();
        }

        for (uint64_t i = 0; i < process; ++i)
        {
            pLeft = system.next();
            points->add_point(k, pLeft.get_x1(), k, pLeft.get_x2());
        }
    }
    return points;
}