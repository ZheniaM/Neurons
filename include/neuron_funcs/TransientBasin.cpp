#include "TransientBasin.hpp"

#include "../funcs.hpp"
#include "../SystemOf2NeuronsStateV2.hpp"

#include <type_traits>
#include <math.h>

std::unique_ptr<Points2> TransientBasin::get_points(void)
{
    auto points = std::make_unique<Points2>();

    printf("PStable: %s\n", static_cast<std::string>(PStable).c_str());
    using Y = std::remove_const_t<decltype(ny1)>;
    using X = std::remove_const_t<decltype(nx1)>;
    using T = std::remove_const_t<decltype(maxt)>;

    for (Y i = 0; i < ny1; ++i)
    {
        for (X j = 0; j < nx1; ++j)
        {
            Point2x2 PStart{
                convertFromTo(j, 0, nx1 - 1, minx1, maxx1),
                convertFromTo(i, 0, ny1 - 1, miny1, maxy1),
                PStable.get_x2(),
                PStable.get_y2(),
            };

            P0 = PStart;
            SystemOf2NeuronsStateV2 system(this);
            T t = 0;
            Point2x2 curr(0, 0);
            do
            {
                curr = system.next();
            } while (++t < maxt && (curr - PStable).mod() >= 1e-8);

            bool isConverge = (t < maxt);
            points->add_point(PStart.get_x1(), PStart.get_y1(), isConverge, t);
        }
    }

    return points;
}