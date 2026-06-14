#pragma once

#include "include/neuron_funcs/Drawer.hpp"
#include "include/Modes.hpp"
#include "include/Point2x2.hpp"
#include "include/SystemOf2NeuronsState.hpp"

#include <TQObject.h>
#include <RQ_OBJECT.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TGWindow.h>
#include <TApplication.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TGNumberEntry.h>
#include <TGComboBox.h>
#include <TGTextView.h>

#define GROUP_LIST     \
    GROUP_AXIS         \
    GROUP_ABC          \
    GROUP_P0           \
    GROUP_SYS_PARAMS   \
    GROUP_PASS_PROCESS \
    GROUP_ADDITIONAL

#define GROUP_AXIS                                                             \
    INIT_GROUP(AXIS, "Axis")                                                   \
    GROUP_ELEM_DOUBLE(AXIS, xAxisLabelSize, "X labels size", 0.08)             \
    GROUP_ELEM_INT(AXIS, xAxisLabelDivisions, "X label div", 1010)             \
    GROUP_ELEM_BOOL(AXIS, xAxisLabelDivisionsOptim, "X label div optim", true) \
    GROUP_ELEM_DOUBLE(AXIS, xAxisTitleSize, "X title size", 0.10)              \
    GROUP_ELEM_DOUBLE(AXIS, xAxisTitleOffset, "X title offset", 0.3)           \
    GROUP_ELEM_BOOL(AXIS, xAxisTitleAtCenter, "X centering title", false)      \
    GROUP_ELEM_DOUBLE(AXIS, yAxisLabelSize, "Y labels size", 0.08)             \
    GROUP_ELEM_INT(AXIS, yAxisLabelDivisions, "Y label divisions", 1010)       \
    GROUP_ELEM_BOOL(AXIS, yAxisLabelDivisionsOptim, "Y label div optim", true) \
    GROUP_ELEM_DOUBLE(AXIS, yAxisTitleSize, "Y title size", 0.10)              \
    GROUP_ELEM_DOUBLE(AXIS, yAxisTitleOffset, "Y title offset", 0.4)           \
    GROUP_ELEM_BOOL(AXIS, yAxisTitleAtCenter, "Y centering title", false)

#define GROUP_ABC                        \
    INIT_GROUP(ABC, "a, b, c")           \
    GROUP_ELEM_DOUBLE(ABC, a, "a", 0.89) \
    GROUP_ELEM_DOUBLE(ABC, b, "b", 0.18) \
    GROUP_ELEM_DOUBLE(ABC, c, "c", 0.28)

#define GROUP_P0                          \
    INIT_GROUP(P0, "P0")                  \
    GROUP_ELEM_DOUBLE(P0, x1, "x1", 0.93) \
    GROUP_ELEM_DOUBLE(P0, y1, "y1", 0.77) \
    GROUP_ELEM_DOUBLE(P0, x2, "x2", 1.30) \
    GROUP_ELEM_DOUBLE(P0, y2, "y2", 1.40)

#define GROUP_SYS_PARAMS                                \
    INIT_GROUP(SystemParams, "system params")           \
    GROUP_ELEM_DOUBLE(SystemParams, I, "I", 0.022)      \
    GROUP_ELEM_DOUBLE(SystemParams, I1, "I1", 0.0)      \
    GROUP_ELEM_DOUBLE(SystemParams, I2, "I2", 0.1)      \
    GROUP_ELEM_DOUBLE(SystemParams, k1, "k1", 0)        \
    GROUP_ELEM_DOUBLE(SystemParams, k2, "k2", 0.045)    \
    GROUP_ELEM_DOUBLE(SystemParams, k, "k", 0.025)      \
    GROUP_ELEM_DOUBLE(SystemParams, k_2, "k_2", 0.030)  \
    GROUP_ELEM_DOUBLE(SystemParams, k_3, "k_3", 0.035)  \
    GROUP_ELEM_DOUBLE(SystemParams, eps, "eps", 1e-4)   \
    GROUP_ELEM_DOUBLE(SystemParams, eps2, "eps2", 1e-3) \
    GROUP_ELEM_BOOL(SystemParams, isStachastick, "is stachastick", false)

#define GROUP_PASS_PROCESS                            \
    INIT_GROUP(PassProcess, "Pass & Process")         \
    GROUP_ELEM_INT(PassProcess, pass, "pass", 10'000) \
    GROUP_ELEM_INT(PassProcess, process, "process", 1'000)

#define GROUP_ADDITIONAL                                                            \
    INIT_GROUP(Additional, "Additional")                                            \
    GROUP_ELEM_DOUBLE(Additional, minx1, "min x1", 0)                               \
    GROUP_ELEM_DOUBLE(Additional, miny1, "min y1", 0)                               \
    GROUP_ELEM_DOUBLE(Additional, maxx1, "max x1", 3)                               \
    GROUP_ELEM_DOUBLE(Additional, maxy1, "max y1", 4)                               \
    GROUP_ELEM_INT(Additional, nx1, "# x1", 100)                                    \
    GROUP_ELEM_INT(Additional, ny1, "# y1", 100)                                    \
    GROUP_ELEM_INT(Additional, onWitch, "t", 100)                                   \
    GROUP_ELEM_DOUBLE(Additional, mineps, "min eps", 1e-4)                          \
    GROUP_ELEM_DOUBLE(Additional, maxeps, "max eps", 1e-3)                          \
    GROUP_ELEM_INT(Additional, neps, "# eps", 10'000)                               \
    GROUP_ELEM_BOOL(Additional, connectDots, "Connect dots", false)                 \
    GROUP_ELEM_BOOL(Additional, addPortretToBassin, "Add portret to bassin", false) \
    GROUP_ELEM_BOOL(Additional, drawWithColorPaletteAtRight, "Draw with color palette at right", false)

struct MyMainFrame
{
    MyMainFrame(TGWindow const *p, UInt_t w = 8 + 1600 + 8 + 248 + 8, UInt_t h = 8 + 960 + 8);
    virtual ~MyMainFrame();
    void DoDraw();
    void DoPrint();
    void DoSetStablePoint();
    void onSelectedComboBox_mode(int id);
    void DoHideAxisGroup();

    static auto constexpr get_stable_point(void) { return SystemOf2NeuronsState::stablePoint; }

private:
    RQ_OBJECT("MyMainFrame")
    TGMainFrame *fMainFrame = nullptr;
    TRootEmbeddedCanvas *fECanvas = nullptr;

public:
    TRootEmbeddedCanvas const *get_eCanvas(void) const { return fECanvas; }

private:
    // режим программы
    // biffurc, time siries, etc.
    TGGroupFrame *group_Mode;
    TGComboBox *fComboBox_mode = nullptr;
    std::function<Drawer::Drawer(MyMainFrame const &mmf)> drawer = nullptr;
    Drawer::Drawer returnedDrawer;

    ////////////////////////////////////////////////////////////////////////////////

#define INIT_GROUP(name, txt) \
private:                      \
    TGGroupFrame *group_##name;
#define GROUP_ELEM_DOUBLE(groupName, name, txt, val) \
private:                                             \
    TGLabel *fLabel_##name = nullptr;                \
    TGNumberEntry *fNumberEntry_##name = nullptr;    \
                                                     \
public:                                              \
    double get_##name(void) const { return fNumberEntry_##name->GetNumber(); }

#define GROUP_ELEM_INT(groupName, name, txt, val) \
private:                                          \
    TGLabel *fLabel_##name = nullptr;             \
    TGNumberEntry *fNumberEntry_##name = nullptr; \
                                                  \
public:                                           \
    uint64_t get_##name(void) const { return fNumberEntry_##name->GetIntNumber(); }

#define GROUP_ELEM_BOOL(groupName, name, txt, val) \
private:                                           \
    TGCheckButton *fCheckButton_##name = nullptr;  \
                                                   \
public:                                            \
    bool get_##name(void) const { return fCheckButton_##name->GetState() != EButtonState::kButtonUp; }

    GROUP_LIST
#undef GROUP_ELEM_BOOL
#undef GROUP_ELEM_INT
#undef GROUP_ELEM_DOUBLE
#undef INIT_GROUP

    ////////////////////////////////////////////////////////////////////////////////

    // TGTextButton *fTextButton_clear;
    // TGTextButton *fTextButton_addContent;
    TGTextButton *fTextButton_hideAxisGroup;
    TGTextButton *fTextButton_draw;
    TGTextButton *fTextButton_print;
    TGTextButton *fTextButton_setStablePoint;

    // std::array<Drawer::Drawer, 4> fGraphObjs;
    // void clear_graph_obj(void);

    void make_all_fields_invisible(void);
#define MODES_X(mode, txt, classname) \
    void make_visible_fields_for_##mode(void);
    MODES
#undef MODES_X

private:
    Point2x2 unstablePoint{0.93, 0.77, 1.3, 1.4};
    bool fIsAxisHiden = false;
    bool isStable = false;
};

#undef DECLARE_LABEL_NE_GETTER_OF_DOUBLE
#undef DECLARE_LABEL_NE_GETTER_OF_INT