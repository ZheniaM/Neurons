// #pragma once
#ifndef __MODES_HPP
#define __MODES_HPP

#include <TRootEmbeddedCanvas.h>
#include <TMultiGraph.h>
#include <TGraph2D.h>
#include <TCanvas.h>
#include <stdint.h>
#include <memory>

#include "Points2.hpp"
#include "../Modes.hpp"

class MyMainFrame;

namespace Drawer
{
    struct Drawer
    {
        char const *drawOption = "";
        char const *name = "NoName";
        char const *args = "";
        union
        {
            // std::unique_ptr<TMultiGraph> mg = nullptr;
            TMultiGraph *mg = nullptr;
            TGraph2D *g2d;
        };

        inline Drawer();
        inline Drawer(char const *const drawOptrion, char const *args, char const *name, std::shared_ptr<Points2> points);
        inline Drawer(char const *const drawOptrion, char const *args, char const *name);
        inline Drawer(Drawer &&drawer);
        inline Drawer &operator=(Drawer &&drawer);
        inline ~Drawer();

        bool has_value() const { return hasValue; }

        Drawer(Drawer const &drawer) = delete;
        Drawer &operator=(Drawer const &) = delete;

    private:
        bool ismg = true;
        bool hasValue = true;
    };

#define MODES_X(mode, txt, classname) \
    Drawer draw_##classname(MyMainFrame const &mmf);
    // Drawer draw_##classname(MyMainFrame const &mmf);
    MODES;
#undef MODES_X

    Drawer::Drawer::~Drawer()
    {
        // printf("~Drawer(), mg=%p\n", mg);
        if (mg && ismg)
            delete mg;
        else if (g2d && !ismg)
            delete g2d;
    }

    Drawer::Drawer(char const *const drawOption, char const *args, char const *name)
        : drawOption(drawOption), name(name), mg(new TMultiGraph), args(args)
    {
        // printf("Drawer(char const *, char const *), mg=%p\n", mg);
    }

    Drawer &Drawer::Drawer::operator=(Drawer &&drawer)
    {
        this->~Drawer();
        // printf("operator=(&&), mg=%p, drawer.mg=%p\n", mg, drawer.mg);
        this->drawOption = std::exchange(drawer.drawOption, "");
        this->mg = std::exchange(drawer.mg, nullptr);
        this->args = std::exchange(drawer.args, "");
        this->name = std::exchange(drawer.name, "NoName");
        this->hasValue = true;
        return *this;
    }

    Drawer::Drawer(Drawer &&drawer)
        : mg(std::exchange(drawer.mg, nullptr)),
          args(std::exchange(drawer.args, "")),
          name(std::exchange(drawer.name, "")),
          drawOption(std::exchange(drawer.drawOption, ""))
    {
        // printf("Drawer(Drawer&&), mg=%p\n", mg);
    }

    Drawer::Drawer() : hasValue(false)
    {
        // printf("Drawer(), mg=%p\n", mg);
    }

    Drawer::Drawer(char const *const drawOption, char const *args, char const *name, std::shared_ptr<Points2> points)
        : drawOption(drawOption), name(name), args(args),
          g2d(new TGraph2D(points->size())),
          ismg(false)
    {
        std::add_const_t<decltype(points->size())> size = points->size();
        for (std::remove_const_t<decltype(size)> i = 0; i < size; ++i)
        {
            double x = points->get_x1_at(i);
            double y = points->get_y1_at(i);
            double z = points->get_x2_at(i);
            bool xinvalid = std::isnan(x) || std::isinf(x);
            bool yinvalid = std::isnan(y) || std::isinf(y);
            bool zinvalid = std::isnan(z) || std::isinf(z);
            if (xinvalid || yinvalid || zinvalid)
                continue;

            g2d->AddPoint(x, y, z);
            // printf("%lf\t%lf\t%lf\n", x, y, z);
        }
    }
}
#endif // __MODES_HPP