#include "Lyapunov2D.hpp"
#include "Lyapunov.hpp"

#include "../SystemOf2NeuronsStateV2.hpp"
#include "../funcs.hpp"

#include <TCanvas.h>
#include <TGraph.h>

#include <thread>

std::unique_ptr<Points2> Lyapunov2D::get_points()
{
    auto points = std::make_unique<Points2>();
    points->reserve(nk * nI);
    constexpr int nthreads = 16;
    std::vector<std::thread> threads;
    threads.reserve(nthreads);
    std::array<Points2, nthreads> results;
    for (int i = 0; i < nthreads; ++i)
    {
        threads.emplace_back(
            [](Lyapunov2D *l2d, int i, Points2 *result)
            {
                for (std::remove_const_t<decltype(l2d->nI)> j = i * l2d->nI / nthreads; j < (i + 1) * l2d->nI / nthreads; ++j)
                {
                    double I = convertFromTo(j, 0, l2d->nI - 1, l2d->I1, l2d->I2);
                    Lyapunov lyapunov{
                        .P0{l2d->P0},
                        .dP{
                            1,
                            1,
                            1,
                            1,
                        },
                        .a{l2d->a},
                        .b{l2d->b},
                        .c{l2d->c},
                        .I{I},
                        .k1{l2d->k1},
                        .k2{l2d->k2},
                        .process{l2d->process},
                        .nk{l2d->nk},
                    };
                    auto pnts = lyapunov.get_points();
                    std::add_const_t<decltype(pnts->size())> size = pnts->size();
                    for (std::remove_const_t<decltype(size)> i = 0; i < size; ++i)
                    {
                        double k = pnts->get_x_at(i);
                        double L = pnts->get_y_at(i);
                        result->add_point(k, I, L, L);
                    }
                }
            },
            this, i, &results[i]);
    }

    // for (std::remove_const_t<decltype(nI)> j = 0; j < nI; ++j)
    // {
    //     I = convertFromTo(j, 0, nI - 1, I1, I2);
    //     Lyapunov lyapunov{
    //         .P0{P0},
    //         .dP{
    //             1,
    //             1,
    //             1,
    //             1,
    //         },
    //         .a{a},
    //         .b{b},
    //         .c{c},
    //         .I{I},
    //         .k1{k1},
    //         .k2{k2},
    //         .process{process},
    //         .nk{nk},
    //     };
    //     auto pnts = lyapunov.get_points();
    //     std::add_const_t<decltype(pnts->size())> size = pnts->size();
    //     for (std::remove_const_t<decltype(size)> i = 0; i < size; ++i)
    //     {
    //         double k = pnts->get_x_at(i);
    //         double L = pnts->get_y_at(i);
    //         points->add_point(k, I, L, L);
    //     }
    // }
    for (std::thread &t : threads)
    {
        t.join();
    }
    for (Points2 const &p: results)
    {
        auto size = p.size();
        for (decltype(size) j = 0; j < size; ++j)
        {
            using T = decltype(p.get_x1_at(j));
            T x1 = p.get_x1_at(j);
            T x2 = p.get_x2_at(j);
            T y1 = p.get_y1_at(j);
            T y2 = p.get_y2_at(j);
            points->add_point(x1, y1, x2, y2);
        }
    }
    return points;
}