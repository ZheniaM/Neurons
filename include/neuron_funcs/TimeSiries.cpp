#include "TimeSiries.hpp"

#include "../funcs.hpp"
#include "../SystemOf2NeuronsStateV2.hpp"

#include <TCanvas.h>
#include <TGraph.h>

#include <iostream>

std::unique_ptr<Points2> TimeSiries::get_points()
{
    auto points = std::make_unique<Points2>();
    points->reserve(process);

    SystemOf2NeuronsStateV2 system(this);
    // printf("[TS]    %d, %e\n", this->isStochastick, this->eps);
    // printf("[So2NS] %d, %e\n", system.get_isStachastick(), system.get_eps());

    uint64_t t = 0;
    for (; t < pass; ++t)
    {
        Point2x2 p = system.next();
    }

    for (t = 0; t < process; ++t)
    {
        Point2x2 p = system.next();
        points->add_point(p.get_x1(), p.get_y1(), p.get_x2(), p.get_y2());
    }

    std::cout << "[TS] last point: " << static_cast<std::string>(system.next()) << std::endl;
    return points;
}