#include "settings.hpp"

#include "SystemOf2NeuronsState.hpp"

namespace settings::portrait
{
    EColor const color = EColor::kMagenta;
    EColor const colorR = EColor::kGreen;
    bool const drawMPortraits = false;
    double const I = 0.045;

    int64_t const sigma1 = 0;
    int64_t const sigma2 = 5500;
    int64_t const inc = 1;
    int64_t const ratio = 100'000;

    uint64_t const pass = 9'000;
    uint64_t const draw = 1'000;
}

namespace settings::graphic
{
    EColor const X1_COLOR = EColor::kRed;
    EColor const X2_COLOR = EColor::kBlue;

    uint64_t const CANVAS_WIDTH = 1980;
    uint64_t const CANVAS_HEIGHT = 1485;

    Style_t const MARKER_STYLE = 20;
    Size_t const MARKER_SIZE = 1.3;
    Width_t const LINE_WIDTH = 2;
}

namespace settings
{
#ifdef SPAIN_VARIANT

    bool const isStachastick = true;
    // Point2x2 const P0(0.1, 0.0, 0.2, 0.0);

    double const I = 0.03;
    double const a = 0.92;
    double const b = 0.6;
    double const c = 0.28;
    // double const sigma = 0.006;
    double const sigma = 0.025;

    double const BIF_RANGE_START = 0.0;
    double const BIF_RANGE_END = 0.04;
    // double const BIF_RANGE_END = 0.002;

    // double const EPS = 0.003;
    double const EPS = 0.0014;
    uint64_t const TS_FROM = 10'000;
    uint64_t const TS_TO = 10'200;


    Point2x2 const P0(SystemOf2NeuronsState(0.0, 0.0));

#else

    // bool const isStachastick = true;
    bool const isStachastick = false;

    double const a = 0.89;
    double const b = 0.18;
    double const c = 0.28;

    double const I = 0.022;
    // double const I = 0.045; // 22-цикл
    double const sigma = 0.025;

    double const EPS = 0.003;
    // double const EPS = 1e-8;

    uint64_t const TS_FROM = 10'000;
    uint64_t const TS_TO = 11'000;
    double const BIF_RANGE_START = 0.0;
    // double const BIF_RANGE_END = 0.1;
    double const BIF_RANGE_END = 0.06;
    // double const BIF_RANGE_END = 1.0;
    // double const BIF_RANGE_END = 0.004;

    // Point2x2 const P0(SystemOf2NeuronsState(0.0, 0.0));
    Point2x2 const P0(1, 2, 1, 1);
#endif
}