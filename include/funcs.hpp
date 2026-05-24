#pragma once

#include <TGraph.h>
#include <string>
#include "Point2x2.hpp"

class TCanvas;
class TGWindow;

// convert x from [a, b] to [c, d]
double convertFromTo(double const x, double const a, double const b, double const c, double const d);
double convertFromToLogy(double const x, double const a, double const b, double const c, double const d);
void set_graphic_style(TGraph &g);
void set_graphic_color(TGraph &g, Color_t c);

template <typename... Args>
std::string string_format(const std::string &format, Args... args)
{
    int size = snprintf(nullptr, 0, format.c_str(), args...) + 1; // +1 для '\0'
    if (size <= 0)
    {
        return "";
    }

    std::vector<char> buffer(size);
    snprintf(buffer.data(), size, format.c_str(), args...);
    return std::string(buffer.data(), buffer.data() + size - 1); // -1 чтобы исключить '\0'
}

// мат ожидание
double mean(std::vector<double> const &data);
// дисперсия
double variance(std::vector<double> const &data);
// R - синхронизация
double sync_measure_R(std::vector<Point2x2> const &points);

void print_canvas(TCanvas *canvas, std::string dir, std::string name, TGWindow *window = nullptr);
