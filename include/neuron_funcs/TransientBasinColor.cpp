#include "TransientBasinColor.hpp"

#include "../funcs.hpp"
#include "../SystemOf2NeuronsStateV2.hpp"

#include <type_traits>
#include <math.h>

// DOSNT USED


std::unique_ptr<Points2> TransientBasinColor::get_points(void)
{
    auto points = std::make_unique<Points2>();

    Point2x2 P00{P0};
    // double xStart, yStart;
    using S = std::remove_const_t<decltype(ny1)>;

    for (S i = 0; i < ny1; ++i)
    {
        // yStart = convertFromTo(i, 0, ny1 - 1, miny1, maxy1);
        for (S j = 0; j < nx1; ++j)
        {
            // xStart = convertFromTo(j, 0, nx1 - 1, minx1, maxx1);
            Point2x2 PStart(
                convertFromTo(j, 0, nx1 - 1, minx1, maxx1),
                convertFromTo(i, 0, ny1 - 1, miny1, maxy1),
                P00.get_x2(), P00.get_y2());

            P0 = PStart;
            SystemOf2NeuronsStateV2 system(this);
                
            std::remove_const_t<decltype(maxt)> t = 0;

            Point2x2 prev(0,0);
            Point2x2 curr(0,0);
            do {
                prev = curr;
                curr = system.next();
                t = system.get_t();
            } while (t < maxt && (prev - curr).mod() >= 1e-8);

            bool isconverge = (t >= maxt);d
            points->add_point(P0.get_x1(), P0.get_y1(), isconverge, t);
            // points->add_point(P0);
        }
    }

    return points;
}