#include "funcs.hpp"

// #include "settings.hpp"
#include "literalOperators.hpp"

#include <numeric>
#include <TCanvas.h>
#include <TImage.h>
#include <TFile.h>
#include <TGFileDialog.h>

double convertFromTo(double const x, double const a, double const b, double const c, double const d)
{
    return c + (x - a) * ((d - c) / (b - a));
}

double convertFromToLogy(double const x, double const a, double const b, double const c, double const d)
{
    return c * std::pow(d / c, x / (b - a));
}

void set_graphic_style(TGraph &g)
{
    g.SetMarkerStyle(20);
    g.SetMarkerSize(1.3);
    g.SetLineWidth(2);
    g.SetBit(TGraph::kCannotPick);
}

void set_graphic_color(TGraph &g, Color_t c)
{
    g.SetMarkerColor(c);
    g.SetLineColor(c);
}

// мат ожидание
double mean(std::vector<double> const &data)
{
    return std::accumulate(data.begin(), data.end(), 0.0) / (double)data.size();
}

// дисперсия
double variance(std::vector<double> const &data)
{
    double avg = mean(data);
    double var = 0.0;
    for (double d : data)
    {
        var += (d - avg) * (d - avg);
    }
    return var / static_cast<double>(data.size() - 1ULL);
}

double sync_measure_R(std::vector<Point2x2> const &points)
{
    std::vector<double> xMeanTime{}, x1s{}, x2s{};
    xMeanTime.reserve(points.size());
    x1s.reserve(points.size());
    x2s.reserve(points.size());

    for (Point2x2 const &p : points)
    {
        double x1 = p.get_x1();
        double x2 = p.get_x2();
        x1s.emplace_back(x1);
        x2s.emplace_back(x2);
        xMeanTime.emplace_back((x1 + x2) / 2.0);
    }

    double varXMeanTime = variance(xMeanTime);
    double varX1 = variance(x1s);
    double varX2 = variance(x2s);

    double R = 2.0 * varXMeanTime / (varX1 + varX2);
    return R;
}

void print_canvas(TCanvas *canvas, std::string dir, std::string name, TGWindow *window)
{
    const char *filetypes[] = {
        "PNG files",
        "*.png",
        0,
        0,
    };
    TGFileInfo fileinfo;
    fileinfo.fFileTypes = filetypes;
    fileinfo.SetIniDir(("Images/"_s + dir).c_str());
    fileinfo.SetFilename((name + ".png"_s).c_str());
    new TGFileDialog(gClient->GetRoot(), window, kFDSave, &fileinfo);

    if (fileinfo.fFilename != nullptr && strlen(fileinfo.fFilename) > 0)
    {
        TImage *img = TImage::Create();

        if (!img)
        {
            printf("[\e[31mprint_canvas\e[0m] Save failed\n");
        }
        else
        {
            img->FromPad(canvas);
            img->WriteImage(fileinfo.fFilename, TImage::EImageFileTypes::kPng);
            printf("[\e[32mprint_canvas\e[0m] File saved %s\n", fileinfo.fFilename);
            delete img;
        }
    }
}