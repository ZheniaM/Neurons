#include "LyapunovEps.hpp"
// #include "Lyapunov.hpp"

#include "../funcs.hpp"
#include "../SystemOf2NeuronsStateV2.hpp"

#include <type_traits>
#include <random>

std::unique_ptr<Points2> LyapunovEps::get_points(void)
{
    auto points = std::make_unique<Points2>();
    points->reserve(neps);

    {
        using PASS = std::remove_const_t<decltype(pass)>;
        SystemOf2NeuronsStateV2 s{this};
        for (PASS p = 0; p < pass; ++p)
        {
            P0 = s.next();
        }
    }
    printPoint2x2(P0);

    using EPS = std::remove_const_t<decltype(neps)>;

    double x1{P0.get_x1()}, y1{P0.get_y1()}, x2{P0.get_x2()}, y2{P0.get_y2()};
    double x1Next{0}, y1Next{0}, x2Next{0}, y2Next{0};
    double xx1{0}, yy1{0}, xx2{0}, yy2{0};
    double xx1Next{0}, yy1Next{0}, xx2Next{0}, yy2Next{0};
    double ksi1{0}, ksi2{0};
    constexpr double DELTA = 1e-6;
    srand(std::random_device()());

    for (EPS _eps = 0; _eps < neps; ++_eps)
    {
        eps = eps1 * std::pow(eps2 / eps1, static_cast<double>(_eps) / (static_cast<double>(neps - 1)));
        // eps = convertFromTo(_eps, 0, neps - 1, eps1, eps2);

        xx1 = x1 + dP.get_x1() * DELTA / dP.mod();
        yy1 = y1 + dP.get_y1() * DELTA / dP.mod();
        xx2 = x2 + dP.get_x2() * DELTA / dP.mod();
        yy2 = y2 + dP.get_y2() * DELTA / dP.mod();
        double sum = 0.0;

        using PROCESS = std::remove_const_t<decltype(process)>;
        for (PROCESS p = 0; p < process; ++p)
        {
            double r1 = (double)rand() / (double)RAND_MAX;
            double r2 = (double)rand() / (double)RAND_MAX;
            ksi1 = std::sqrt(-2.0 * std::log(r1)) * std::sin(2.0 * M_PI * r2);
            ksi2 = std::sqrt(-2.0 * std::log(r1)) * std::cos(2.0 * M_PI * r2);
            x1Next = x1 * x1 * std::exp(y1 - x1) + I + k * (x2 - x1) + eps * ksi1;
            x2Next = x2 * x2 * std::exp(y2 - x2) + I + k * (x1 - x2) + eps * ksi2;
            y1Next = a * y1 - b * x1 + c;
            y2Next = a * y2 - b * x2 + c;

            xx1Next = xx1 * xx1 * std::exp(yy1 - xx1) + I + k * (xx2 - xx1) + eps * ksi1;
            xx2Next = xx2 * xx2 * std::exp(yy2 - xx2) + I + k * (xx1 - xx2) + eps * ksi2;
            yy1Next = a * yy1 - b * xx1 + c;
            yy2Next = a * yy2 - b * xx2 + c;

            double dx1 = xx1Next - x1Next;
            double dy1 = yy1Next - y1Next;
            double dx2 = xx2Next - x2Next;
            double dy2 = yy2Next - y2Next;
            double mod = std::sqrt(dx1 * dx1 + dy1 * dy1 + dx2 * dx2 + dy2 * dy2);
            sum += std::log(mod / DELTA);

            double px1 = dx1 * DELTA / mod;
            double py1 = dy1 * DELTA / mod;
            double px2 = dx2 * DELTA / mod;
            double py2 = dy2 * DELTA / mod;

            x1 = x1Next;
            y1 = y1Next;
            x2 = x2Next;
            y2 = y2Next;

            xx1 = x1 + px1;
            yy1 = y1 + py1;
            xx2 = x2 + px2;
            yy2 = y2 + py2;
        }
        points->add_point(eps, sum / (double)process, eps, sum / (double)process);
    }

    return points;
}