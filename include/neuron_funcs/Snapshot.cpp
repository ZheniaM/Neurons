#include "Snapshot.hpp"

#include "../SystemOf2NeuronsStateV2.hpp"
#include "../funcs.hpp"

std::unique_ptr<Points> Snapshot::get_points(void)
{
    auto points = std::make_unique<Points>();
    SystemOf2NeuronsStateV2 startSystem(this);
    for (uint64_t i = 0; i < 1'000'000; ++i)
    {
        startSystem.next();
    }
    points->add_point(startSystem.get_x1(), startSystem.get_y1());
    double x2 = startSystem.get_x2();
    double y2 = startSystem.get_y2();
    printf("[Snapshot] x2 = %.8lf, y2 = %.8lf\n", x2, y2);

    for (uint64_t xx1 = 0; xx1 < nx1; ++xx1)
    {
        for (uint64_t yy1 = 0; yy1 < ny1; ++yy1)
        {
            double const x1 = convertFromTo(xx1, 0, nx1 - 1, minx1, maxx1);
            double const y1 = convertFromTo(yy1, 0, ny1 - 1, miny1, maxy1);
            P0 = Point2x2(x1, y1, x2, y2);
            SystemOf2NeuronsStateV2 system(this);

            for (uint64_t tt = 0; tt < t; ++tt)
            {
                system.next();
            }

            points->add_point(system.get_x1(), system.get_y1());
        }
    }

    return points;
}