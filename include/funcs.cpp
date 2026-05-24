#include "funcs.hpp"

// #include "settings.hpp"
#include "literalOperators.hpp"

#include <numeric>
#include <TCanvas.h>
#include <TImage.h>
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
    //     g.SetMarkerStyle(settings::graphic::MARKER_STYLE);
    //     g.SetMarkerSize(settings::graphic::MARKER_SIZE);
    //     g.SetLineWidth(settings::graphic::LINE_WIDTH);
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
    // (R < 0.0) && (R = 0.0);
    // (R > 1.0) && (R = 1.0);
    return R;
}

// double sync_measure_R(std::vector<Point2x2> const &points)
// {
//     double varTime = 0.0;
//     double meanTime = 0.0;
//     double varOverall = 0.0;
//     double meanOverall = 0.0;

//     double const n = points.size();

//     for (Point2x2 const &p : points)
//     {
//         double x1 = p.get_x1();
//         double x2 = p.get_x2();
//         double avg = (x1 + x2) / 2.0;

//         meanOverall += x1;
//         meanOverall += x2;
//         varOverall += x1 * x1;
//         varOverall += x2 * x2;

//         meanTime += avg;
//         varTime += avg * avg;
//     }

//     varOverall -= meanOverall;
//     // varOverall /= static_cast<double>(2 * n - 1);

//     varTime -= meanTime;
//     // varTime /= static_cast<double>(n - 1);

//     double ret = varTime / varOverall;
//     ret *= static_cast<double>(n - 1) / static_cast<double>(2 * n - 1);
//     return ret;
// }

// double sync_measure_R(std::vector<Point2x2> const &points)
// {
//     std::size_t const num_points = points.size();
//     if (num_points <= 0) return 0.0;
    
//     // Step 1: Calculate time means for each neuron
//     double mean_x1 = 0.0, mean_x2 = 0.0;
    
//     for (int i = 0; i < num_points; i++) {
//         mean_x1 += points[i].get_x1();
//         mean_x2 += points[i].get_x2();
//     }
//     mean_x1 /= num_points;
//     mean_x2 /= num_points;
    
//     // Step 2: Calculate variance over time for x1 and x2
//     double var_x1 = 0.0, var_x2 = 0.0;
    
//     for (int i = 0; i < num_points; i++) {
//         double diff1 = points[i].get_x1() - mean_x1;
//         double diff2 = points[i].get_x2() - mean_x2;
//         var_x1 += diff1 * diff1;
//         var_x2 += diff2 * diff2;
//     }
//     var_x1 /= num_points;
//     var_x2 /= num_points;
    
//     // Step 3: Calculate Var(x) = variance over time for combined data
//     // We need the variance of the entire dataset
//     double sum_x = 0.0, sum_x2 = 0.0;
//     std::size_t const total_samples = num_points * 2; // Two neurons at each time point
    
//     for (int i = 0; i < num_points; i++) {
//         sum_x += points[i].get_x1() + points[i].get_x2();
//         sum_x2 += points[i].get_x1() * points[i].get_x1() + points[i].get_x2() * points[i].get_x2();
//     }
    
//     double mean_x = sum_x / total_samples;
//     double var_x = (sum_x2 / total_samples) - (mean_x * mean_x);
    
//     // Step 4: Calculate mean average over all neurons at each time
//     double* mean_at_time = new double[num_points];
//     double sum_mean_at_time = 0.0;
    
//     for (int i = 0; i < num_points; i++) {
//         mean_at_time[i] = (points[i].get_x1() + points[i].get_x2()) / 2.0;
//         sum_mean_at_time += mean_at_time[i];
//     }
    
//     // Step 5: Calculate variance of the mean (Var(bar{x}))
//     double mean_of_means = sum_mean_at_time / num_points;
//     double var_mean = 0.0;
    
//     for (int i = 0; i < num_points; i++) {
//         double diff = mean_at_time[i] - mean_of_means;
//         var_mean += diff * diff;
//     }
//     var_mean /= num_points;
    
//     // Clean up
//     delete[] mean_at_time;
    
//     // Step 6: Calculate R(x)
//     // Avoid division by zero
//     if (var_x == 0.0) return 1.0; // If no variance, fully synchronized
    
//     double R = var_mean / var_x;
    
//     // Ensure R is in [0, 1] as per the formula
//     if (R < 0.0) return 0.0;
//     if (R > 1.0) return 1.0;
    
//     return R;
// }



#include <TFile.h>
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