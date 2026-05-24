#include "MyMainFrame.hpp"
#include <TApplication.h>
#include <TGClient.h>
#include <TStyle.h>
#include <TROOT.h>
#include <random>

#include "include/Palette.hpp"

int main(int argc, char *argv[])
{
    srand(std::random_device()());
    TApplication app("App", &argc, argv);
    gStyle->SetCanvasPreferGL(false);
    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    PaletteInfo.init();
    // gROOT
    new MyMainFrame(gClient->GetRoot());
    app.Run(true);
}
