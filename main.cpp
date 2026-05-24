#include <TCanvas.h>
#include <TColor.h>
#include <TFile.h>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TLine.h>
#include <TMultiGraph.h>
#include <TStyle.h>
#include <TView.h>
#include <TH1D.h>
#include <TMarker.h>
#include <TLegend.h>

#include <math.h>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <fstream>

#include "include/settings.hpp"
#include "include/SystemOf2NeuronsState.hpp"
#include "include/funcs.hpp"

void get_time_siries_of_neurns(std::vector<Point2x2> &outPoints, uint64_t const N = settings::TS_TO, Point2x2 const &P0 = settings::P0)
{
    SystemOf2NeuronsState system(P0, settings::sigma, settings::I, settings::a, settings::b, settings::c);
    outPoints.reserve(N);
    outPoints.push_back(P0);

    for (uint64_t i = 1; i < N; ++i)
    {
        outPoints.emplace_back(system.next());
    }
}

void draw_time_siries(std::vector<Point2x2> &points, uint64_t const nFrom = settings::TS_FROM)
{
    TCanvas canvas("b", "b", 201, 10, settings::graphic::CANVAS_WIDTH, settings::graphic::CANVAS_HEIGHT);
    TGraph g1, g2;
    set_graphic_color(g1, settings::graphic::X1_COLOR);
    set_graphic_color(g2, settings::graphic::X2_COLOR);
    set_graphic_style(g1);
    set_graphic_style(g2);

    uint64_t t = nFrom;
    std::for_each(
        points.begin() + nFrom, points.end(),
        [&g1, &g2, &t](Point2x2 const &p)
        {
            g1.AddPoint(t, p.get_x1());
            g2.AddPoint(t, p.get_x2());
            t++;
        });

    std::string args = string_format("#sigma=%0.4lf, #varepsilon=%1.2e", settings::sigma, settings::EPS);

    TMultiGraph mg;
    mg.SetTitle(string_format("time siries, %s;t;x1,x2", args.c_str()).c_str());
    mg.Add(&g1), mg.Add(&g2);

    mg.GetYaxis()->SetLabelSize(0.03);
    mg.Draw("ALP");
    canvas.Print(string_format("ts/ts: %s.png", args.c_str()).c_str());
}

void bifurc(void)
{
    TCanvas canvas("b", "b", 200, 10, settings::graphic::CANVAS_WIDTH, settings::graphic::CANVAS_HEIGHT);
    canvas.Divide(1, 2);
    TGraph gx1, gx2;

    // Point2x2 const startPoint(SystemOf2NeuronsState(0.0, 0.0));
    // Point2x2 const startPoint(0.1, 1, 0.2, 0.9);
    Point2x2 const startPoint(settings::P0);
    uint64_t const npass = 10'000;
    uint64_t const ndraw = settings::graphic::CANVAS_HEIGHT / 2;
    uint64_t const nsigma = settings::graphic::CANVAS_WIDTH;

    printf("pass: %lx, draw: %lx, nk: %lx\n", npass, ndraw, nsigma);

    std::ofstream f("main_sys");
    for (uint64_t s = 0; s < nsigma; ++s)
    {
        double const sigma = convertFromTo(s, 0, nsigma - 1, settings::BIF_RANGE_START, settings::BIF_RANGE_END);
        SystemOf2NeuronsState system(startPoint, sigma, settings::I, settings::a, settings::b, settings::c);
        // SystemOf2NeuronsState system(startPoint, sigma, 0.045, settings::a, settings::b, settings::c);
        system.setIsStochastick(false);

        for (uint64_t i = 0; i < npass; ++i)
        {
            Point2x2 p = system.next();
            // if (s == 0 && 0 == i % 1000)
            // {
            //     std::cout << p.to_string() << std::endl;
            // }
        }
        f << static_cast<std::string>(system) << std::endl;
        for (uint64_t i = 0; i < ndraw; ++i)
        {
            Point2x2 p = system.next();
            gx1.AddPoint(sigma, p.get_x1());
            gx2.AddPoint(sigma, p.get_x2());
        }
        f << static_cast<std::string>(system) << std::endl;
        // std::cout << system.to_string() << std::endl;
    }
    f.close();
    gx1.GetXaxis()->SetLabelSize(0.05);
    gx1.GetYaxis()->SetLabelSize(0.05);
    gx1.GetXaxis()->SetTitleSize(0.05);
    gx1.GetYaxis()->SetTitleSize(0.05);

    gx2.GetXaxis()->SetLabelSize(0.05);
    gx2.GetYaxis()->SetLabelSize(0.05);
    gx2.GetXaxis()->SetTitleSize(0.05);
    gx2.GetYaxis()->SetTitleSize(0.05);

    std::string args = string_format("I=%0.6lf, #sigma_{1}=%0.4lf, #sigma_{2}=%0.4lf",
                                     settings::I,
                                     settings::BIF_RANGE_START,
                                     settings::BIF_RANGE_END);
    canvas.cd(1);
    gx1.SetTitle(string_format("Bifurcation X1: %s;#sigma;x1", args.c_str()).c_str());
    gx1.SetMarkerColor(settings::graphic::X1_COLOR);
    gx1.Draw("AP");

    canvas.cd(2);
    gx2.SetTitle(string_format("Bifurcation X2: %s;#sigma;x1", args.c_str()).c_str());
    gx2.SetMarkerColor(settings::graphic::X2_COLOR);
    gx2.Draw("AP");

    canvas.Print(string_format("bifs/bif: %s.png", args.c_str()).c_str());
}

double portrait(double const sigma, bool const draw = true)
{
    double const I = settings::portrait::I;
    SystemOf2NeuronsState system(settings::P0, sigma, I, settings::a, settings::b, settings::c);
    system.setIsStochastick(false);

    for (uint64_t i = 0; i < settings::portrait::pass; ++i)
    {
        system.next();
    }

    TCanvas canvas("b", "b", 201, 10, settings::graphic::CANVAS_WIDTH, settings::graphic::CANVAS_HEIGHT);
    TGraph g;

    std::vector<Point2x2> ps;
    ps.reserve(settings::portrait::draw);
    for (uint64_t i = 0; i < settings::portrait::draw; ++i)
    {
        Point2x2 p = system.next();
        ps.push_back(p);
        g.AddPoint(p.get_x1(), p.get_x2());
    }
    double R = sync_measure_R(ps);
    if (!draw)
        return R;

    set_graphic_style(g);
    set_graphic_color(g, settings::portrait::color);

    std::string args = string_format("I=%0.4lf, #sigma=%0.4lf, where R=%0.4lf", I, sigma, R);

    g.SetTitle(string_format("portrait: %s;X1;X2", args.c_str()).c_str());

    g.Draw("AP");
    canvas.Print(string_format("portraits/portrait: %s.png", args.c_str()).c_str());

    return R;
}

// draw many portraits + sigma-R sync graph
void mportraits(void)
{
    TCanvas canvas("b", "b", 201, 10, settings::graphic::CANVAS_WIDTH, settings::graphic::CANVAS_HEIGHT);
    TGraph g;

    for (uint64_t s = settings::portrait::sigma1;
         s < settings::portrait::sigma2;
         s += settings::portrait::inc)
    {
        double sigma = (double)s / settings::portrait::ratio;
        double R = portrait(sigma, settings::portrait::drawMPortraits);
        g.AddPoint(sigma, R);
    }

    set_graphic_style(g);
    g.SetMarkerSize(0.5);
    set_graphic_color(g, settings::portrait::colorR);

    std::string args = string_format("I=%0.4lf, d#sigma=%0.2e, #sigma_{1}=%0.4lf, #sigma_{2}=%0.4lf",
                                     settings::portrait::I,
                                     (double)settings::portrait::inc / settings::portrait::ratio,
                                     (double)settings::portrait::sigma1 / settings::portrait::ratio,
                                     (double)settings::portrait::sigma2 / settings::portrait::ratio);

    g.SetTitle(string_format("R(X | #sigma), %s;#sigma;R", args.c_str()).c_str());

    g.Draw("AP");
    canvas.Print(string_format("portraits/R-sigma: %s, .png", args.c_str()).c_str());
}

void print_help(void)
{
    printf(
        "--bif : bifurcation diagram\n"
        "--ts  : time siries\n"
        "--portrait [sigma] : portrait with givin sigma arg\n"
        "--mportraits: many portraits (see settings::portrait)\n");
}

int main(int argc, char *argv[])
{
    printf("a: %lx, b: %lx, c: %lx\n",
           reinterpret_cast<int64_t const &>(settings::a),
           reinterpret_cast<int64_t const &>(settings::b),
           reinterpret_cast<int64_t const &>(settings::c));
    printf("sigma: %lx, I: %lx\n",
           reinterpret_cast<int64_t const &>(settings::sigma),
           reinterpret_cast<int64_t const &>(settings::I));
    printf("k1: %lx, k2: %lx\n",
           reinterpret_cast<int64_t const &>(settings::BIF_RANGE_START),
           reinterpret_cast<int64_t const &>(settings::BIF_RANGE_END));
    std::cout << static_cast<std::string>(SystemOf2NeuronsState(settings::P0, settings::sigma, settings::I, settings::a, settings::b, settings::c)) << std::endl;
    if (1 == argc)
    {
        print_help();
        return 0;
    }

    if (argv[1][0] != '-' || argv[1][1] != '-')
    {
        print_help();
        return 0;
    }
    std::string flag = argv[1] + 2;

    std::map<std::string, std::function<void(void)>> hash;
    hash["bif"] = &bifurc;

    hash["ts"] = []()
    {
        std::vector<Point2x2> ps;
        get_time_siries_of_neurns(ps);
        draw_time_siries(ps);
        std::vector<Point2x2> cutPoints(ps.begin() + settings::TS_FROM, ps.end());
        // double sync = sync_measure_R(cutPoints);
        double sync = sync_measure_R(ps);
        printf("R(x) = %1.6lf\n", sync);
    };

    hash["portrait"] = [argv]()
    { portrait(std::stod(argv[2])); };

    hash["mportraits"] = &mportraits;

    std::function<void(void)>
        f = hash[flag];
    if (NULL == f)
        print_help();
    else
        f();
}