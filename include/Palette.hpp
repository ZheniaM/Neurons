#pragma once

#include <cuchar>
#include <TColor.h>
#include <TStyle.h>

static constexpr std::size_t PaletteSize = 200;
struct
{
    const std::size_t size = PaletteSize;
    int get_color_at(std::size_t i) { return paletteStart + i; }

    void init(void)
    {
        static bool isInit = false;
        if (isInit)
        {
            gStyle->SetPalette(size, palette);
            return;
        }   
        double Red[] = {0., 0.0, 1.0, 1.0};
        double Green[] = {0., 0.0, 1.0, 0.0};
        double Blue[] = {0., 1.0, 0.0, 0.0};
        double Length[] = {0., .33, .66, 1.0};
        paletteStart = TColor::CreateGradientColorTable((sizeof(Length) / sizeof(double)), Length, Red, Green, Blue, size);
        for (std::remove_const_t<decltype(size)> i = 0; i < size; i++)
            palette[i] = paletteStart + i;
        gStyle->SetPalette(size, palette);
    }

private:
    int paletteStart;
    int palette[PaletteSize];
} PaletteInfo;
