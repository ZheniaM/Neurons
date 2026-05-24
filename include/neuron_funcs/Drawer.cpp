#include "Drawer.hpp"
#include "Points2.hpp"
#include "../../MyMainFrame.hpp"
#include "../funcs.hpp"
#include "../Palette.hpp"

#include <TCanvas.h>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TLine.h>
#include <TLatex.h>
#include <TH1.h>

#include "Bifurc.hpp"
#include "BifurcTraction.hpp"
#include "Portrait.hpp"
#include "SyncR.hpp"
#include "TimeSiries.hpp"
#include "Lyapunov.hpp"
#include "Snapshot.hpp"
#include "StochastickLevel.hpp"
#include "SyncR2D.hpp"
#include "Lyapunov2D.hpp"
#include "TransientBasin.hpp"
#include "BifurcI.hpp"
#include "Lyapunov2DChaos.hpp"
#include "SyncR2DChaos.hpp"
#include "LyapunovEps.hpp"
#include "StabilityOfChaos.hpp"

Drawer::Drawer Drawer::draw_Bifurc(MyMainFrame const &mmf)
{
    Bifurc bifurc{
        .P0{mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2()},
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},
        .k1{mmf.get_k1()},
        .k2{mmf.get_k2()},
        .I{mmf.get_I()},
        .pass{mmf.get_pass()},
        .process{mmf.get_process()},
        .nk{mmf.get_eCanvas()->GetWidth()},
    };
    auto points = bifurc.get_points();
    auto constexpr stable = mmf.get_stable_point();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    canvas->Divide(1, 2);

    char const *args = Form("I=%0.6lf", bifurc.I);
    // char const *args = Form("I=%0.6lf, k_{1}=%0.4lf, k_{2}=%0.4lf",
    //                         bifurc.I, bifurc.k1, bifurc.k2);

    Width_t constexpr lineWidth = 2;
    TLine linex{bifurc.k1, stable.lf_x, bifurc.k2, stable.lf_x};
    linex.SetLineWidth(lineWidth);

    TLine liney{bifurc.k1, stable.lf_y, bifurc.k2, stable.lf_y};
    liney.SetLineWidth(lineWidth);

    TGraph gx1(points->size(), points->get_data_x1s(), points->get_data_y1s());
    TGraph gx2(points->size(), points->get_data_x2s(), points->get_data_y2s());
    Drawer drawer1("AP", args, "Bifurc");
    Drawer drawer2("AP", args, "Bifurc");
    auto mg1 = drawer1.mg;
    auto mg2 = drawer2.mg;

    gx1.GetXaxis()->SetLabelSize(0.05);
    gx1.GetYaxis()->SetLabelSize(0.05);
    gx1.GetXaxis()->SetTitleSize(0.05);
    gx1.GetYaxis()->SetTitleSize(0.05);

    gx2.GetXaxis()->SetLabelSize(0.05);
    gx2.GetYaxis()->SetLabelSize(0.05);
    gx2.GetXaxis()->SetTitleSize(0.05);
    gx2.GetYaxis()->SetTitleSize(0.05);

    canvas->cd(1);
    gx1.SetMarkerColor(EColor::kRed);
    mg1->SetTitle(Form("Bifurcation X_{1}: %s;k;X_{1}", args));
    mg1->Add(&gx1);
    mg1->Draw(drawer1.drawOption);
    linex.Draw();

    canvas->cd(2);
    gx2.SetMarkerColor(EColor::kBlue);
    mg2->SetTitle(Form("Bifurcation Y_{1}: %s;k;Y_{1}", args));
    mg2->Add(&gx2);
    mg2->Draw(drawer2.drawOption);
    liney.Draw();

    canvas->Update();
    return drawer2;
}

Drawer::Drawer Drawer::draw_BifurcTraction(MyMainFrame const &mmf)
{
    BifurcTraction bt{
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},
        .k1{mmf.get_k1()},
        .k2{mmf.get_k2()},
        .kStart{mmf.get_k()},
        .I{mmf.get_I()},
        .pass{mmf.get_pass()},
        .process{mmf.get_process()},
        .nk{mmf.get_eCanvas()->GetWidth()},
    };
    auto points = bt.get_points();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    canvas->Divide(1, 2);
    char const *args = Form("I=%0.6lf, k_{1}=%0.4lf, k_{2}=%0.4lf, k_{start}=%0.4lf",
                            bt.I, bt.k1, bt.k2, bt.kStart);

    TGraph gx1(points->size(), points->get_data_x1s(), points->get_data_y1s());
    TGraph gx2(points->size(), points->get_data_x2s(), points->get_data_y2s());
    Drawer drawer1("AP", args, "BifurcTraction");
    Drawer drawer2("AP", args, "BifurcTraction");
    auto mg1 = drawer1.mg;
    auto mg2 = drawer2.mg;

    gx1.GetXaxis()->SetLabelSize(0.05);
    gx1.GetYaxis()->SetLabelSize(0.05);
    gx1.GetXaxis()->SetTitleSize(0.05);
    gx1.GetYaxis()->SetTitleSize(0.05);

    gx2.GetXaxis()->SetLabelSize(0.05);
    gx2.GetYaxis()->SetLabelSize(0.05);
    gx2.GetXaxis()->SetTitleSize(0.05);
    gx2.GetYaxis()->SetTitleSize(0.05);
    canvas->cd(1);
    gx1.SetMarkerColor(EColor::kRed);
    mg1->SetTitle(Form("Bifurcation by traction X_{1}: %s;k;X_{1}", args));
    mg1->Add(&gx1);
    mg1->Draw(drawer1.drawOption);

    canvas->cd(2);
    gx2.SetMarkerColor(EColor::kBlue);
    mg2->SetTitle(Form("Bifurcation by traction X_{2}: %s;k;X_{2}", args));
    mg2->Add(&gx2);
    mg2->Draw(drawer2.drawOption);

    canvas->Update();
    return drawer1;
}

Drawer::Drawer Drawer::draw_Portrait(MyMainFrame const &mmf)
{
    Portrait portrait{
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},
        .I{mmf.get_I()},
        .k{mmf.get_k()},
        .pass{mmf.get_pass()},
        .process{mmf.get_process()},
        .isStochastick{mmf.get_isStachastick()},
        .eps{mmf.get_eps()},
    };
    auto points = portrait.get_points();

    auto constexpr stable = mmf.get_stable_point();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    canvas->Divide(2);
    char const *args = portrait.isStochastick
                           ? Form("I=%0.4lf, k=%0.4lf, #varepsilon=%1.0e", portrait.I, portrait.k, portrait.eps)
                           : Form("I=%0.4lf, k=%0.4lf", portrait.I, portrait.k);
    EColor constexpr color = EColor::kMagenta;
    bool const connectDots = mmf.get_connectDots();
    char const *const drawopt = connectDots ? "APL" : "AP";

    canvas->cd(1);
    TGraph gxx0;
    gxx0.AddPoint(stable.lf_x, stable.lf_x);
    gxx0.SetMarkerStyle(EMarkerStyle::kFullCircle);
    gxx0.SetMarkerSize(2);
    TGraph gxx(points->size(), points->get_data_x1s(), points->get_data_x2s());
    Drawer drawerxx(drawopt, args, "Portrait");
    auto mgxx = drawerxx.mg;
    set_graphic_style(gxx);
    gxx.SetMarkerColor(color);
    // gxx.GetXaxis()->SetTitleSize(0.03);

    mgxx->SetTitle(Form("Portrait: %s;X1;X2", args));
    mgxx->Add(&gxx);
    mgxx->Add(&gxx0);
    // mgxx->Draw(drawerxx.drawOption);
    mgxx->Draw("A");

    // canvas->Modified();
    canvas->Update();
    auto histxx = mgxx->GetHistogram();
    histxx->GetXaxis()->SetLimits(0.0, 3.0);
    histxx->SetMinimum(0.0);
    histxx->SetMaximum(3.0);
    mgxx->Draw(connectDots ? "PL SAME" : "P SAME");
    canvas->Update();
    // mgxx->GetXaxis()->SetLimits(0.0, 3.0);
    // mgxx->SetMinimum(0.0);
    // mgxx->SetMaximum(3.0);

    canvas->cd(2);
    TGraph gxy0;
    gxy0.AddPoint(stable.lf_x, stable.lf_y);
    gxy0.SetMarkerStyle(EMarkerStyle::kFullCircle);
    gxy0.SetMarkerSize(2);
    TGraph gxy(points->size(), points->get_data_x1s(), points->get_data_y1s());
    Drawer drawerxy(drawopt, args, "Portrait");
    auto mgxy = drawerxy.mg;
    set_graphic_style(gxy);
    gxy.SetMarkerColor(color);

    mgxy->SetTitle(Form("Portrait: %s;X1;Y1", args));
    mgxy->Add(&gxy);
    mgxy->Add(&gxy0);
    mgxy->Draw("A");

    canvas->Modified();
    canvas->Update();
    auto histxy = mgxy->GetHistogram();
    histxy->GetXaxis()->SetLimits(0.0, 3.0);
    histxy->SetMinimum(0.0);
    histxy->SetMaximum(4.0);
    mgxy->Draw(connectDots ? "PL SAME" : "P SAME");
    canvas->Update();
    // mgxy->GetXaxis()->SetLimits(0.0, 3.0);
    // mgxy->SetMinimum(0.0);
    // mgxy->SetMaximum(4.0);

    return drawerxx;
}

Drawer::Drawer Drawer::draw_SyncR(MyMainFrame const &mmf)
{
    SyncR syncr{
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},
        .k1{mmf.get_k1()},
        .k2{mmf.get_k2()},
        .I{mmf.get_I()},
        .pass{mmf.get_pass()},
        .process{mmf.get_process()},
        .nk{mmf.get_eCanvas()->GetWidth()},
        .eps{mmf.get_eps()},
        .isStachastick{mmf.get_isStachastick()},
    };

    auto points = syncr.get_points();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    char const *args = Form("I=%0.4lf, k_{1}=%0.4lf, k_{2}=%0.4lf",
                            syncr.I, syncr.k1, syncr.k2);

    TGraph g(points->size(), points->get_data_xs(), points->get_data_ys());
    Drawer drawer("AP", args, "SyncR");
    auto mg = drawer.mg;

    set_graphic_style(g);
    set_graphic_color(g, EColor::kGreen);
    g.SetMarkerSize(0.5);

    mg->SetTitle(Form("Sync R: %s;K;R", args));

    mg->Add(&g);
    mg->Draw(drawer.drawOption);
    canvas->Update();
    return drawer;
}

Drawer::Drawer Drawer::draw_TimeSiries(MyMainFrame const &mmf)
{
    TimeSiries ts{
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},
        .k{mmf.get_k()},
        .I{mmf.get_I()},
        .pass{mmf.get_pass()},
        .process{mmf.get_process()},
        .isStochastick{mmf.get_isStachastick()},
        .eps{mmf.get_eps()},
    };

    auto points = ts.get_points();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    canvas->Divide(1, 2);
    char const *args;
    if (ts.isStochastick)
        args = Form("k=%0.4lf, #varepsilon=%1.2e", ts.k, ts.eps);
    else
        args = Form("k=%0.4lf", ts.k);

    double t[points->size()];
    for (size_t i = 0; i < points->size(); ++i)
        t[i] = i;

    canvas->cd(1);
    TGraph gx1(points->size(), t, points->get_data_x1s());
    TGraph gx2(points->size(), t, points->get_data_x2s());
    Drawer drawer1("ALP", args, "TimeSiries");
    TMultiGraph *mgx = drawer1.mg;

    set_graphic_style(gx1);
    set_graphic_style(gx2);
    gx1.SetMarkerSize(0.9);
    gx2.SetMarkerSize(0.9);
    set_graphic_color(gx1, EColor::kRed);
    set_graphic_color(gx2, EColor::kBlue);

    mgx->SetTitle(Form("Time siries: %s;T;X_{1}, X_{2}", args));
    mgx->Add(&gx1), mgx->Add(&gx2);
    mgx->Draw("A");
    canvas->Update();
    mgx->SetMinimum(0.0);
    mgx->SetMaximum(6.0);
    mgx->Draw("LP SAME");
    canvas->Update();


    canvas->cd(2);
    TGraph gy1(points->size(), t, points->get_data_y1s());
    TGraph gy2(points->size(), t, points->get_data_y2s());
    Drawer drawer2("ALP", args, "TimeSiries");
    TMultiGraph *mgy = drawer2.mg;

    set_graphic_style(gy1);
    set_graphic_style(gy2);
    gy1.SetMarkerSize(0.9);
    gy2.SetMarkerSize(0.9);
    set_graphic_color(gy1, EColor::kRed);
    set_graphic_color(gy2, EColor::kBlue);

    mgy->SetTitle(Form("Time siries: %s;T;Y_{1}, Y_{2}", args));
    mgy->Add(&gy1), mgy->Add(&gy2);
    mgy->Draw("A");
    canvas->Update();
    mgy->SetMinimum(0);
    mgy->SetMaximum(2.5);
    mgy->Draw("LP SAME");
    canvas->Update();
    return drawer1;
}

Drawer::Drawer Drawer::draw_TimeSiriesTwoEps(MyMainFrame const &mmf)
{
    TimeSiries ts1{
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},
        .k{mmf.get_k()},
        .I{mmf.get_I()},
        .pass{mmf.get_pass()},
        .process{mmf.get_process()},
        .isStochastick{true},
        .eps{mmf.get_eps()},
    };

    auto points1 = ts1.get_points();

    TimeSiries ts2{
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},
        .k{mmf.get_k()},
        .I{mmf.get_I()},
        .pass{mmf.get_pass()},
        .process{mmf.get_process()},
        .isStochastick{true},
        .eps{mmf.get_eps2()},
    };

    auto points2 = ts2.get_points();
    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    canvas->Divide(1, 2);
    char const *args;
    args = Form("k=%0.4lf, #varepsilon_{1}=%1.0e, #varepsilon_{2}=%1.0e", ts1.k, ts1.eps, ts2.eps);

    double t[points1->size()];
    for (size_t i = 0; i < points1->size(); ++i)
        t[i] = i;

    canvas->cd(1);
    TGraph gx1(points1->size(), t, points1->get_data_x1s());
    TGraph gx2(points2->size(), t, points2->get_data_x1s());
    Drawer drawer1("ALP", args, "TimeSiries");
    TMultiGraph *mgx = drawer1.mg;

    set_graphic_style(gx1);
    set_graphic_style(gx2);
    gx1.SetMarkerSize(0.9);
    gx2.SetMarkerSize(0.9);
    set_graphic_color(gx1, EColor::kRed);
    set_graphic_color(gx2, EColor::kBlue);

    mgx->SetTitle(Form("Time siries: %s;T;X_{1}", args));
    mgx->Add(&gx1), mgx->Add(&gx2);
    mgx->Draw(drawer1.drawOption);

    canvas->cd(2);
    TGraph gy1(points1->size(), t, points1->get_data_y1s());
    TGraph gy2(points2->size(), t, points2->get_data_y1s());
    Drawer drawer2("ALP", args, "TimeSiries");
    TMultiGraph *mgy = drawer2.mg;

    set_graphic_style(gy1);
    set_graphic_style(gy2);
    gy1.SetMarkerSize(0.9);
    gy2.SetMarkerSize(0.9);
    set_graphic_color(gy1, EColor::kRed);
    set_graphic_color(gy2, EColor::kBlue);

    mgy->SetTitle(Form("Time siries: %s;T;Y_{1}", args));
    mgy->Add(&gy1), mgy->Add(&gy2);
    mgy->Draw(drawer2.drawOption);

    canvas->Update();
    // print_canvas(canvas, "TS", "test");
    return drawer1;
}

Drawer::Drawer Drawer::draw_Lyapunov(MyMainFrame const &mmf)
{
    Lyapunov lyapunov{
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .dP{
            1,
            1,
            1,
            1,
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},
        .I{mmf.get_I()},
        .k1{mmf.get_k1()},
        .k2{mmf.get_k2()},
        // .pass{mmf.get_pass()},
        .process{mmf.get_process()},
        .nk{mmf.get_eCanvas()->GetWidth()},
    };

    auto points = lyapunov.get_points();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    char const *args = Form("I=%0.4lf, k_{1}=%0.4lf, k_{2}=%0.4lf",
                            lyapunov.I, lyapunov.k1, lyapunov.k2);

    TGraph g(points->size(), points->get_data_xs(), points->get_data_ys());
    Drawer drawer("ALP", args, "Lyapunov");
    auto mg = drawer.mg;

    set_graphic_style(g);
    set_graphic_color(g, EColor::kGreen);
    g.SetMarkerSize(0.5);

    mg->SetTitle(Form("Lyapunov: %s;K;#lambda", args));
    mg->Add(&g);
    mg->Draw(drawer.drawOption);

    canvas->Update();
    return drawer;
}

Drawer::Drawer Drawer::draw_Snapshot(MyMainFrame const &mmf)
{
    Snapshot snapshot{
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},
        .I{mmf.get_I()},
        .k{mmf.get_k()},
        .minx1{mmf.get_minx1()},
        .miny1{mmf.get_miny1()},
        .maxx1{mmf.get_maxx1()},
        .maxy1{mmf.get_maxy1()},
        .nx1{mmf.get_nx1()},
        .ny1{mmf.get_ny1()},
        .t{mmf.get_onWitch()},
    };
    auto points = snapshot.get_points();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    char const *args = Form("I=%0.4lf, k=%0.4lf, t=%lu",
                            snapshot.I, snapshot.k, snapshot.t);

    canvas->RangeAxis(snapshot.minx1, snapshot.miny1,
                      snapshot.maxx1, snapshot.maxy1);
    TGraph g(points->size(), points->get_data_xs(), points->get_data_ys());
    TGraph g0(1, points->get_data_xs(), points->get_data_ys());
    Drawer drawer("AP", args, "Snapshot");
    TMultiGraph *mg = drawer.mg;

    double xRanges[] = {snapshot.minx1, snapshot.maxx1};
    double yRanges[] = {snapshot.miny1, snapshot.maxy1};
    TGraph gRanges(2, xRanges, yRanges);
    set_graphic_color(gRanges, EColor::kWhite);

    // gRanges.AddPoint(snapshot.minx1, snapshot.miny1);
    // gRanges.AddPoint(snapshot.maxx1, snapshot.maxy1);

    set_graphic_style(g);
    set_graphic_color(g, EColor::kViolet);
    g0.SetMarkerStyle(20);
    g0.SetMarkerSize(3);
    set_graphic_color(g0, EColor::kBlack);

    mg->Add(&g), mg->Add(&g0), mg->Add(&gRanges);
    mg->SetTitle(Form("Snapshot: %s;X_{1};Y_{1}", args));
    mg->GetXaxis()->SetRangeUser(snapshot.minx1, snapshot.maxx1);
    mg->GetYaxis()->SetRangeUser(snapshot.miny1, snapshot.maxy1);
    mg->Draw(drawer.drawOption);

    canvas->Update();
    return drawer;
}

Drawer::Drawer Drawer::draw_StochastickLevel(MyMainFrame const &mmf)
{
    StochastickLevel sl{
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},
        .k{mmf.get_k()},
        .I{mmf.get_I()},
        .pass{mmf.get_pass()},
        .process{mmf.get_process()},
        .mineps{mmf.get_mineps()},
        .maxeps{mmf.get_maxeps()},
        .neps{mmf.get_neps()},
    };

    auto points = sl.get_points();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    char const *args = Form("k=%.4lf, I=%.4lf", sl.k, sl.I);
    TLatex tex(1e-4, 1, args);
    tex.SetTextSize(0.08);
    // char const *args = Form("k=%.4lf, #varepsilon_{min}=%1.2e, #varepsilon_{max}=%1.2e", sl.k, sl.mineps, sl.maxeps);
    canvas->Divide(1, 2);

    canvas->cd(1);
    canvas->cd(1)->SetLogx();
    canvas->cd(1)->SetLogy();
    TGraph gx(points->size(), points->get_data_x1s(), points->get_data_y1s());
    Drawer drawer1("AP", args, "StochastickLevel");
    TMultiGraph *mgx = drawer1.mg;

    set_graphic_color(gx, EColor::kBlue);
    mgx->Add(&gx);
    mgx->SetTitle(Form("Stochastick level: %s;#varepsilon;X_{1}", args));
    mgx->Draw(drawer1.drawOption);
    tex.Draw();

    canvas->cd(2);
    canvas->cd(2)->SetLogx();
    canvas->cd(2)->SetLogy();
    TGraph gy(points->size(), points->get_data_x2s(), points->get_data_y2s());
    Drawer drawer2("AP", args, "StochastickLevel");
    TMultiGraph *mgy = drawer2.mg;

    set_graphic_color(gy, EColor::kRed);
    mgy->Add(&gy);
    mgy->SetTitle(Form("Stochastick level: %s;#varepsilon;Y_{1}", args));
    mgy->Draw(drawer2.drawOption);
    tex.Draw();

    canvas->Update();
    return drawer1;
}

Drawer::Drawer Drawer::draw_SyncR2D(MyMainFrame const &mmf)
{
    SyncR2D syncr2d{
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},

        .I1{mmf.get_I1()},
        .I2{mmf.get_I2()},
        .nI{mmf.get_eCanvas()->GetHeight()},

        .k1{mmf.get_k1()},
        .k2{mmf.get_k2()},
        .nk{mmf.get_eCanvas()->GetWidth()},

        .pass{mmf.get_pass()},
        .process{mmf.get_process()},
    };

    std::shared_ptr<Points2> points = syncr2d.get_points();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    canvas->SetTheta(90);
    canvas->SetPhi(1e-6);
    char const *args = Form("I_{1}=%0.4lf, I_{2}=%0.4lf, k_{1}=%0.4lf, k_{2}=%0.4lf",
                            syncr2d.I1, syncr2d.I2, syncr2d.k1, syncr2d.k2);

    Drawer drawer("PCOLZ", args, "SyncR2D", points);
    auto g2d = drawer.g2d;
    // g2d->RemoveDuplicates();
    g2d->SetTitle(Form("Sync R 2D: %s;K;I", args));
    g2d->SetMarkerStyle(EMarkerStyle::kFullDotSmall);
    g2d->SetMarkerSize(2);
    g2d->Draw(drawer.drawOption);
    canvas->Update();
    return drawer;
}

Drawer::Drawer Drawer::draw_Lyapunov2D(MyMainFrame const &mmf)
{
    Lyapunov2D lyapunov2d{
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},

        .I1{mmf.get_I1()},
        .I2{mmf.get_I2()},
        .nI{mmf.get_eCanvas()->GetHeight()},

        .k1{mmf.get_k1()},
        .k2{mmf.get_k2()},
        .nk{mmf.get_eCanvas()->GetWidth()},

        .pass{mmf.get_pass()},
        .process{mmf.get_process()},
    };

    std::shared_ptr<Points2> points = lyapunov2d.get_points();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    canvas->SetTheta(90);
    canvas->SetPhi(1e-6);
    char const *args = Form("I_{1}=%0.4lf, I_{2}=%0.4lf, k_{1}=%0.4lf, k_{2}=%0.4lf",
                            lyapunov2d.I1, lyapunov2d.I2, lyapunov2d.k1, lyapunov2d.k2);

    Drawer drawer("PCOLZ", args, "Lyapunov2D", points);
    auto g2d = drawer.g2d;
    // g2d->RemoveDuplicates();
    g2d->GetXaxis()->SetLabelSize(0.02);
    g2d->GetYaxis()->SetLabelSize(0.02);
    g2d->GetZaxis()->SetLabelSize(0.02);

    g2d->SetTitle(Form("Lyapunov 2D: %s;K;I", args));
    g2d->SetMarkerStyle(EMarkerStyle::kFullDotSmall);
    g2d->SetMarkerSize(2);
    g2d->Draw(drawer.drawOption);
    canvas->Update();
    return drawer;
}

Drawer::Drawer Drawer::draw_TransientBasin(MyMainFrame const &mmf)
{
    TransientBasin tb{
        .PStable{
            mmf.get_stable_point().lf_x,
            mmf.get_stable_point().lf_y,
            mmf.get_stable_point().lf_x,
            mmf.get_stable_point().lf_y,
        },
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},

        .I{mmf.get_I()},
        .k{mmf.get_k()},

        .minx1{mmf.get_minx1()},
        .miny1{mmf.get_miny1()},
        .maxx1{mmf.get_maxx1()},
        .maxy1{mmf.get_maxy1()},

        .nx1{mmf.get_nx1()},
        .ny1{mmf.get_ny1()},
        .maxt{mmf.get_process()},
    };
    auto points = tb.get_points();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    char const *args = Form("I=%0.4lf, k=%0.4lf, x_{1}#in(%0.4lf, %0.4lf), y_{1}#in(%0.4lf, %0.4lf)",
                            tb.I, tb.k, tb.minx1, tb.maxx1, tb.miny1, tb.maxy1);
    Drawer drawer("AP", args, "TransientBasin");
    auto mg = drawer.mg;

    TGraph gconverge, gdiverge;
    auto const size = points->size();
    for (std::remove_const_t<decltype(size)> i = 0; i < size; ++i)
    {
        double x = points->get_x1_at(i);
        double y = points->get_y1_at(i);
        bool isconverge = points->get_x2_at(i);
        // printf("t = %lf\n", points->get_y2_at(i));
        if (isconverge)
            gconverge.AddPoint(x, y);
        else
            gdiverge.AddPoint(x, y);
    }

    gconverge.SetMarkerColor(EColor::kGreen);
    gdiverge.SetMarkerColor(EColor::kOrange);
    gconverge.SetMarkerStyle(EMarkerStyle::kDot);
    gdiverge.SetMarkerStyle(EMarkerStyle::kDot);
    mg->SetTitle(Form("Transient Basin: %s;x_{1};y_{1}", args));
    mg->Add(&gconverge);
    mg->Add(&gdiverge);

    TGraph portret;
    if (mmf.get_addPortretToBassin())
    {
        Portrait prtrt{
            .P0{
                mmf.get_x1(),
                mmf.get_y1(),
                mmf.get_x2(),
                mmf.get_y2(),
            },
            .a{mmf.get_a()},
            .b{mmf.get_b()},
            .c{mmf.get_c()},

            .I{mmf.get_I()},
            .k{mmf.get_k()},

            .pass{mmf.get_pass()},
            .process{mmf.get_process()},
        };
        auto ppoints = prtrt.get_points();
        using S = decltype(ppoints->size());
        for (S i = 0; i < ppoints->size(); ++i)
        {
            portret.AddPoint(ppoints->get_x1_at(i), ppoints->get_y1_at(i));
        }
        portret.SetMarkerStyle(EMarkerStyle::kFullCircle);
        portret.SetMarkerSize(1);
        portret.SetMarkerColor(EColor::kMagenta);
        mg->Add(&portret);
    }

    mg->Draw(drawer.drawOption);
    mg->GetXaxis()->SetLimits(tb.minx1, tb.maxx1);
    mg->SetMinimum(tb.miny1);
    mg->SetMaximum(tb.maxy1);
    canvas->Modified();
    canvas->Update();
    return drawer;
}

Drawer::Drawer Drawer::draw_TransientBasinColor(MyMainFrame const &mmf)
{
    TransientBasin tb{
        .PStable{
            mmf.get_stable_point().lf_x,
            mmf.get_stable_point().lf_y,
            mmf.get_stable_point().lf_x,
            mmf.get_stable_point().lf_y,
        },
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},

        .I{mmf.get_I()},
        .k{mmf.get_k()},

        .minx1{mmf.get_minx1()},
        .miny1{mmf.get_miny1()},
        .maxx1{mmf.get_maxx1()},
        .maxy1{mmf.get_maxy1()},

        .nx1{mmf.get_nx1()},
        .ny1{mmf.get_ny1()},
        .maxt{mmf.get_process()},
    };
    auto points = tb.get_points();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    char const *args = Form("I=%0.4lf, k=%0.4lf, x_{1}#in(%0.4lf, %0.4lf), y_{1}#in(%0.4lf, %0.4lf)",
                            tb.I, tb.k, tb.minx1, tb.maxx1, tb.miny1, tb.maxy1);

    if (!mmf.get_drawWithColorPaletteAtRight()) // no Color palette
    {
        Drawer drawer("AP", args, "TransientBasinColor");
        auto mg = drawer.mg;
        TGraph gs[PaletteInfo.size];
        auto const size = points->size();
        for (std::remove_const_t<decltype(size)> i = 0; i < size; ++i)
        {
            double x = points->get_x1_at(i);
            double y = points->get_y1_at(i);
            int t = points->get_y2_at(i);
            std::size_t idx = convertFromTo(t, 0, tb.maxt, 0, PaletteInfo.size - 1);
            gs[idx].AddPoint(x, y);
        }

        mg->SetTitle(Form("Transient Basin Color: %s;x_{1};y_{1}", args));
        for (std::size_t idx = 0; idx < PaletteInfo.size; ++idx)
        {
            gs[idx].SetMarkerStyle(EMarkerStyle::kDot);
            gs[idx].SetMarkerColor(PaletteInfo.get_color_at(idx));
            gs[idx].SetLineColor(PaletteInfo.get_color_at(idx));
            if (gs[idx].GetN() > 0)
            {
                mg->Add(gs + idx);
            }
        }

        mg->Draw(drawer.drawOption);
        mg->GetXaxis()->SetLimits(tb.minx1, tb.maxx1);
        mg->SetMinimum(tb.miny1);
        mg->SetMaximum(tb.maxy1);
        canvas->Modified();
        canvas->Update();
        return drawer;
    }
    else // with color palette
    {
        TGraph2D g2d;
        auto const size = points->size();
        for (std::remove_const_t<decltype(size)> i = 0; i < size; ++i)
        {
            double x = points->get_x1_at(i);
            double y = points->get_y1_at(i);
            int t = points->get_y2_at(i);
            g2d.AddPoint(x, y, t);
        }
        g2d.AddPoint(tb.minx1, tb.miny1, 0);
        g2d.AddPoint(tb.maxx1, tb.maxy1, tb.maxt);
        g2d.SetTitle(Form("Transient Basin color: I=%0.4lf, k=%0.4lf;X_{1};Y_{1}", tb.I, tb.k));
        Drawer drawer("PCOLZ", args, "TransientBasinColor");
        g2d.Draw(drawer.drawOption);
        canvas->SetTheta(90);
        canvas->SetPhi(1e-6);
        canvas->Update();
        return drawer;
    }
}

Drawer::Drawer Drawer::draw_BifurcI(MyMainFrame const &mmf)
{
    BifurcI bi{
        .P0{mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2()},
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},
        .k{mmf.get_k()},
        .I1{mmf.get_I1()},
        .I2{mmf.get_I2()},
        .pass{mmf.get_pass()},
        .process{mmf.get_process()},
        .nI{mmf.get_eCanvas()->GetWidth()},
    };
    auto points = bi.get_points();
    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    canvas->Divide(1, 2);

    char const *args = Form("k=%0.4lf, I_{1}=%0.4lf, I_{2}=%0.4lf",
                            bi.k, bi.I1, bi.I2);

    TGraph gx1(points->size(), points->get_data_y2s(), points->get_data_x1s());
    TGraph gx2(points->size(), points->get_data_y2s(), points->get_data_y1s());
    Drawer drawer1("AP", args, "BifurcI");
    Drawer drawer2("AP", args, "BifurcI");
    auto mg1 = drawer1.mg;
    auto mg2 = drawer2.mg;

    gx1.GetXaxis()->SetLabelSize(0.05);
    gx1.GetYaxis()->SetLabelSize(0.05);
    gx1.GetXaxis()->SetTitleSize(0.05);
    gx1.GetYaxis()->SetTitleSize(0.05);

    gx2.GetXaxis()->SetLabelSize(0.05);
    gx2.GetYaxis()->SetLabelSize(0.05);
    gx2.GetXaxis()->SetTitleSize(0.05);
    gx2.GetYaxis()->SetTitleSize(0.05);

    canvas->cd(1);
    gx1.SetMarkerColor(EColor::kRed);
    mg1->SetTitle(Form("Bifurcation IOX: %s;I;X_{1}", args));
    mg1->Add(&gx1);
    mg1->Draw(drawer1.drawOption);

    canvas->cd(2);
    gx2.SetMarkerColor(EColor::kBlue);
    mg2->SetTitle(Form("Bifurcation IOY: %s;I;Y_{1}", args));
    mg2->Add(&gx2);
    mg2->Draw(drawer2.drawOption);

    canvas->Update();
    return drawer2;
}

Drawer::Drawer Drawer::draw_Lyapunov2DChaos(MyMainFrame const &mmf)
{
    Lyapunov2DChaos l2dc{
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .dP{1, 2, 3, 4},
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},

        .I{mmf.get_I()},

        .k1{mmf.get_k1()},
        .k2{mmf.get_k2()},
        .nk{mmf.get_eCanvas()->GetWidth() / 2},

        .eps1{mmf.get_mineps()},
        .eps2{mmf.get_maxeps()},
        .neps{mmf.get_eCanvas()->GetHeight() * 5 / 2},

        .pass{mmf.get_pass()},
        .process{mmf.get_process()},
    };
    std::shared_ptr<Points2> points = l2dc.get_points();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    canvas->SetTheta(90);
    canvas->SetPhi(1e-6);
    char const *args = Form("I=%0.4lf, k#in[%0.4lf, %0.4lf], #varepsilon#in[%1.2e, %1.2e]",
                            l2dc.I, l2dc.k1, l2dc.k2, l2dc.eps1, l2dc.eps2);

    canvas->SetLogy();
    // canvas->RangeAxis(l2dc.k1, l2dc.eps1, l2dc.k2, l2dc.eps2);
    Drawer drawer("PCOLZ", args, "Lyapunov2DChaos", points);
    auto g2d = drawer.g2d;
    g2d->GetXaxis()->SetLabelSize(0.02);
    g2d->GetYaxis()->SetLabelSize(0.02);
    g2d->GetZaxis()->SetLabelSize(0.02);

    g2d->SetTitle(Form("Lyapunov 2D: %s;K;#varepsilon", args));
    g2d->SetMarkerStyle(EMarkerStyle::kFullDotSmall);
    g2d->SetMarkerSize(4);
    g2d->Draw(drawer.drawOption);
    // canvas->SetLogy();
    canvas->Update();
    return drawer;
}

Drawer::Drawer Drawer::draw_SyncR2DChaos(MyMainFrame const &mmf)
{
    SyncR2DChaos sr2dc{
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},

        .I{mmf.get_I()},

        .k1{mmf.get_k1()},
        .k2{mmf.get_k2()},
        .nk{mmf.get_eCanvas()->GetWidth()},

        .eps1{mmf.get_mineps()},
        .eps2{mmf.get_maxeps()},
        .neps{mmf.get_eCanvas()->GetHeight()},

        .pass{mmf.get_pass()},
        .process{mmf.get_process()},
    };
    std::shared_ptr<Points2> points = sr2dc.get_points();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    canvas->SetTheta(90);
    canvas->SetPhi(1e-6);
    char const *args = Form("I=%0.4lf, k#in[%0.4lf, %0.4lf], #varepsilon#in[%1.2e, %1.2e]",
                            sr2dc.I, sr2dc.k1, sr2dc.k2, sr2dc.eps1, sr2dc.eps2);

    // canvas->SetLogy();
    canvas->RangeAxis(sr2dc.k1, sr2dc.eps1, sr2dc.k2, sr2dc.eps2);
    Drawer drawer("PCOLZ", args, "SyncR2DChaos", points);
    auto g2d = drawer.g2d;
    g2d->GetXaxis()->SetLabelSize(0.02);
    g2d->GetYaxis()->SetLabelSize(0.02);
    g2d->GetZaxis()->SetLabelSize(0.02);

    g2d->SetTitle(Form("SyncR 2D: %s;K;#varepsilon", args));
    g2d->SetMarkerStyle(EMarkerStyle::kDot);
    // g2d->SetMarkerSize(2);
    g2d->Draw(drawer.drawOption);
    canvas->Update();
    return drawer;

    return drawer;
}

Drawer::Drawer Drawer::draw_LyapunovEps(MyMainFrame const &mmf)
{
    LyapunovEps leps{
        .P0{
            mmf.get_x1(),
            mmf.get_y1(),
            mmf.get_x2(),
            mmf.get_y2(),
        },
        .dP{
            1.0,
            2.0,
            3.0,
            4.0,
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},

        .I{mmf.get_I()},
        .k{mmf.get_k()},

        .eps1{mmf.get_mineps()},
        .eps2{mmf.get_maxeps()},
        .neps{mmf.get_eCanvas()->GetHeight()},

        .pass{mmf.get_pass()},
        .process{mmf.get_process()},
    };

    auto points = leps.get_points();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    char const *args = Form("k=%0.4lf, #varepsilon_{1}=%1.0e, #varepsilon_{2}=%1.0e",
                            leps.k, leps.eps1, leps.eps2);

    TGraph g(points->size(), points->get_data_x1s(), points->get_data_y1s());
    Drawer drawer("ALP", args, "LyapunovEps");
    auto mg = drawer.mg;

    g.SetMarkerStyle(EMarkerStyle::kDot);
    g.SetMarkerColor(EColor::kGreen);
    g.SetLineColor(EColor::kGreen);

    mg->SetTitle(Form("LyapunovEps: %s;#varepsilon;#lambda", args));
    mg->Add(&g);
    mg->Draw("A");
    canvas->Update();
    mg->GetXaxis()->SetLimits(leps.eps1, leps.eps2);
    canvas->SetLogx();
    mg->Draw("LP SAME");
    canvas->Update();
    return drawer;
}

Drawer::Drawer Drawer::draw_StabilityOfChaos(MyMainFrame const &mmf)
{
    StabilityOfChaos sc1{
        .P0{
            mmf.get_stable_point().lf_x,
            mmf.get_stable_point().lf_y,
            mmf.get_stable_point().lf_x,
            mmf.get_stable_point().lf_y,
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},
        .I{mmf.get_I()},
        .k{mmf.get_k()},
        .eps1{mmf.get_mineps()},
        .eps2{mmf.get_maxeps()},
        .neps{mmf.get_neps()},
        .process{mmf.get_process()},
        .isStochastick{true},
    };
    auto points1 = sc1.get_points();

    StabilityOfChaos sc2{
        .P0{
            mmf.get_stable_point().lf_x,
            mmf.get_stable_point().lf_y,
            mmf.get_stable_point().lf_x,
            mmf.get_stable_point().lf_y,
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},
        .I{mmf.get_I()},
        .k{mmf.get_k_2()},
        .eps1{mmf.get_mineps()},
        .eps2{mmf.get_maxeps()},
        .neps{mmf.get_neps()},
        .process{mmf.get_process()},
        .isStochastick{true},
    };
    auto points2 = sc2.get_points();

    StabilityOfChaos sc3{
        .P0{
            mmf.get_stable_point().lf_x,
            mmf.get_stable_point().lf_y,
            mmf.get_stable_point().lf_x,
            mmf.get_stable_point().lf_y,
        },
        .a{mmf.get_a()},
        .b{mmf.get_b()},
        .c{mmf.get_c()},
        .I{mmf.get_I()},
        .k{mmf.get_k_3()},
        .eps1{mmf.get_mineps()},
        .eps2{mmf.get_maxeps()},
        .neps{mmf.get_neps()},
        .process{mmf.get_process()},
        .isStochastick{true},
    };
    auto points3 = sc3.get_points();

    TCanvas *canvas = mmf.get_eCanvas()->GetCanvas();
    char const *args = Form("I=%0.6lf, k=%0.4lf, #varepsilon#in(%1.0e, %1.0e)",
                            sc1.I, sc1.k, sc1.eps1, sc1.eps2);

    TGraph g1{static_cast<Int_t>(points1->size()), points1->get_data_xs(), points1->get_data_ys()};
    TGraph g2{static_cast<Int_t>(points2->size()), points2->get_data_xs(), points2->get_data_ys()};
    TGraph g3{static_cast<Int_t>(points3->size()), points3->get_data_xs(), points3->get_data_ys()};
    Drawer drawer("ALP", args, "StabilityOfChaos");
    auto mg = drawer.mg;
    
    g1.SetMarkerStyle(EMarkerStyle::kDot);
    g1.SetMarkerColor(EColor::kRed);
    g1.SetLineColor(EColor::kRed);
    g1.SetLineWidth(4);

    g2.SetMarkerStyle(EMarkerStyle::kDot);
    g2.SetMarkerColor(EColor::kGreen);
    g2.SetLineColor(EColor::kGreen);
    g2.SetLineWidth(4);

    g3.SetMarkerStyle(EMarkerStyle::kDot);
    g3.SetMarkerColor(EColor::kBlue);
    g3.SetLineColor(EColor::kBlue);
    g3.SetLineWidth(4);

    mg->SetTitle(Form("Stability: %s;#varepsilon;Stability", args));
    mg->Add(&g1);
    mg->Add(&g2);
    mg->Add(&g3);
    mg->Draw("A");
    canvas->Update();
    mg->GetXaxis()->SetLimits(sc1.eps1, sc1.eps2);
    mg->SetMinimum(0.0);
    mg->SetMaximum(1.0);
    canvas->SetLogx();
    mg->Draw("LP SAME");

    TLegend legend{0.1, 0.75, 0.3, 0.9};
    legend.AddEntry(&g1, Form("k = %0.4lf", sc1.k));
    legend.AddEntry(&g2, Form("k = %0.4lf", sc2.k));
    legend.AddEntry(&g3, Form("k = %0.4lf", sc3.k));
    legend.Draw();

    canvas->Update();
    return drawer;
}