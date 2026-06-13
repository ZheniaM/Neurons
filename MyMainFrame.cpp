#include "MyMainFrame.hpp"
#include "include/funcs.hpp"
#include "include/CanvasModes.hpp"
#include "include/SystemOf2NeuronsState.hpp"

// #include <TGFileDialog.h>
// #include <TImage.h>
// #include <TGWindow.h>
// #include <TSystem.h>

class ProtectedCanvas : public TCanvas
{
public:
    ProtectedCanvas(char const *name, Int_t ww, Int_t wh, Int_t winid)
        : TCanvas(name, ww, wh, winid) {}

    void HandleInput(EEventType button, Int_t x, Int_t y) override {}
};

#define CREATE_LABEL_NE(p, name, txt, val, digitw, fmt)                                                         \
    TGHorizontalFrame *hframe_##name = new TGHorizontalFrame(p);                                                \
    (p)->AddFrame(hframe_##name, new TGLayoutHints(kLHintsExpandX));                                            \
    fNumberEntry_##name = new TGNumberEntry(hframe_##name, (val), (digitw), -1, (fmt));                         \
    hframe_##name->AddFrame(fNumberEntry_##name, new TGLayoutHints(kLHintsRight | kLHintsCenterY, 4, 4, 4, 4)); \
    fNumberEntry_##name->Resize(paramsw * 5 / 8, 20);                                                           \
    fLabel_##name = new TGLabel(hframe_##name, (txt));                                                          \
    hframe_##name->AddFrame(fLabel_##name, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 4, 4, 4, 4));        \
    fLabel_##name->Resize(paramsw / 4, 20);

#define CREATE_BUTTON(p, name, txt, callback)                                \
    fTextButton_##name = new TGTextButton((p), "&" txt);                     \
    fTextButton_##name->Connect("Clicked()", "MyMainFrame", this, callback); \
    (p)->AddFrame(fTextButton_##name, new TGLayoutHints(kLHintsExpandX | kLHintsNormal, 0, 0, 8, 0));

#define DECLARE_GROUP_FRAME(p, name, txt)                      \
    group_##name = new TGGroupFrame((p), (txt)); \
    (p)->AddFrame(group_##name, new TGLayoutHints(kLHintsNormal | kLHintsExpandX, 0, 0, 8, 0));

MyMainFrame::MyMainFrame(TGWindow const *p, UInt_t w, UInt_t h)
{
    fMainFrame = new TGMainFrame(p, w, h, kMainFrame | kHorizontalFrame);
    fMainFrame->SetCleanup(kDeepCleanup);
    fMainFrame->Connect("CloseWindow()", "MyMainFrame", this, "~MyMainFrame()");

    ////////////////////
    // Canvas
    ////////////////////

    fECanvas = new TRootEmbeddedCanvas("ECanvas", fMainFrame, 1600, 960, kSunkenFrame, 0x1d2731);
    int idEcanvas = fECanvas->GetCanvasWindowId();
    TCanvas *c = new ProtectedCanvas("b", 1600, 900, idEcanvas);

    c->SetEditable(false);
    c->SetFixedAspectRatio(true);
    c->SetBit(TCanvas::kNoContextMenu);
    c->ToggleToolTips();
    c->SetHighLightColor(0);

    fECanvas->AdoptCanvas(c);
    fMainFrame->AddFrame(fECanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop, 8, 8, 8, 8));

    ////////////////////
    // hframe
    ////////////////////

    uint64_t const paramsw = 248;
    TGVerticalFrame *vframe = new TGVerticalFrame(fMainFrame, paramsw, 960);
    fMainFrame->AddFrame(vframe, new TGLayoutHints(kLHintsNormal, 8, 8, 8, 8));

    ////////////////////
    // mode combo box
    ////////////////////

    DECLARE_GROUP_FRAME(vframe, Mode, "mode");

    fComboBox_mode = new TGComboBox(group_Mode);
#define MODES_X(mode, txt, classname) \
    fComboBox_mode->AddEntry(txt, Modes::id::mode);
    MODES
#undef MODES_X

    group_Mode->AddFrame(fComboBox_mode, new TGLayoutHints(kLHintsRight, 8, 8, 8, 8));
    // fComboBox_mode->MoveResize(8, 16, 216, 20);
    fComboBox_mode->Resize(paramsw, 20);
    fComboBox_mode->Connect("Selected(int)", "MyMainFrame", this, "onSelectedComboBox_mode(int)");

    ////////////////////
    // Every params
    ////////////////////

    CREATE_BUTTON(vframe, hideAxisGroup, "Hide axis group", "DoHideAxisGroup()");
#define INIT_GROUP(name, txt)                                                                      \
    DECLARE_GROUP_FRAME(vframe, name, txt)                                                         \
    TGVerticalFrame *vframe_##name = new TGVerticalFrame(group_##name, paramsw);                   \
    group_##name->AddFrame(vframe_##name);

#define GROUP_ELEM_DOUBLE(groupName, name, txt, val) \
    CREATE_LABEL_NE(group_##groupName, name, txt, val, 1, TGNumberFormat::kNESReal)

#define GROUP_ELEM_INT(groupName, name, txt, val) \
    CREATE_LABEL_NE(group_##groupName, name, txt, val, 12, TGNumberFormat::kNESInteger)

#define GROUP_ELEM_BOOL(groupName, name, txt, val)                   \
    fCheckButton_##name = new TGCheckButton(group_##groupName, txt); \
    fCheckButton_##name->SetState(static_cast<EButtonState>(val));   \
    group_##groupName->AddFrame(fCheckButton_##name, new TGLayoutHints(kLHintsLeft | kLHintsTop, 5, 5, 5, 5));

    GROUP_LIST
#undef GROUP_ELEM_BOOL
#undef GROUP_ELEM_INT
#undef GROUP_ELEM_DOUBLE
#undef INIT_GROUP

    ////////////////////
    // Buttons
    ////////////////////

    CREATE_BUTTON(vframe, setStablePoint, "Switch point", "DoSetStablePoint()");
    CREATE_BUTTON(vframe, draw, "Draw", "DoDraw()");
    CREATE_BUTTON(vframe, print, "Print", "DoPrint()");

    fMainFrame->SetWindowName("window name");
    fMainFrame->MapSubwindows();
    fMainFrame->Resize(fMainFrame->GetDefaultSize());
    fMainFrame->MapWindow();
    fComboBox_mode->Select(0);
    // fComboBox_canvasMode->Select(0);
    fIsAxisHiden = false;
    DoHideAxisGroup();
}

MyMainFrame::~MyMainFrame()
{
    fMainFrame->Cleanup();
    delete fMainFrame;
    gApplication->Terminate(0);
}

void MyMainFrame::DoHideAxisGroup()
{
    fIsAxisHiden ^= 1;
    if (fIsAxisHiden)
    {
        group_AXIS->UnmapWindow();
        fTextButton_hideAxisGroup->SetText("Show axis group");
    }
    else
    {
        group_AXIS->MapWindow();
        fTextButton_hideAxisGroup->SetText("Hide axis group");
    }
    onSelectedComboBox_mode(fComboBox_mode->GetSelected());
}

void MyMainFrame::DoDraw()
{
    TCanvas *canvas = fECanvas->GetCanvas();
    canvas->SetEditable(true);
    canvas->Clear();
    canvas->SetLogx(0);
    canvas->SetLogy(0);
    returnedDrawer = drawer(*this);
    canvas->SetEditable(false);

    canvas->Modified();
}

void MyMainFrame::DoPrint()
{
    print_canvas(fECanvas->GetCanvas(), returnedDrawer.name, returnedDrawer.args, fMainFrame);
}

void MyMainFrame::DoSetStablePoint()
{
    static bool isStable = false;
    static Point2x2 unstablePoint(0.93, 0.77, 1.3, 1.4);
    double x1, y1, x2, y2;
    // union hex2double
    // {
    //     uint64_t u64;
    //     double f64;
    // };
    // hex2double x1;
    // hex2double y1;
    // hex2double x2;
    // hex2double y2;

    if (!isStable)
    {
        x1 = SystemOf2NeuronsState::stablePoint.lf_x;
        y1 = SystemOf2NeuronsState::stablePoint.lf_y;
        x2 = SystemOf2NeuronsState::stablePoint.lf_x;
        y2 = SystemOf2NeuronsState::stablePoint.lf_y;
        // x1.u64 = 0x3fa65a4d262ccbd8;
        // y1.u64 = 0x4003cac83591c392;
        // x2.u64 = 0x3fa65a4d262ccbd8;
        // y2.u64 = 0x4003cac83591c392;
        unstablePoint = Point2x2{
            fNumberEntry_x1->GetNumber(),
            fNumberEntry_y1->GetNumber(),
            fNumberEntry_x2->GetNumber(),
            fNumberEntry_y2->GetNumber(),
        };
    }
    else
    {
        x1 = unstablePoint.get_x1();
        y1 = unstablePoint.get_y1();
        x2 = unstablePoint.get_x2();
        y2 = unstablePoint.get_y2();
        // x1.f64 = unstablePoint.get_x1();
        // y1.f64 = unstablePoint.get_y1();
        // x2.f64 = unstablePoint.get_x2();
        // y2.f64 = unstablePoint.get_y2();
    }
    isStable = !isStable;

    fNumberEntry_x1->SetNumber(x1, true);
    fNumberEntry_y1->SetNumber(y1, true);
    fNumberEntry_x2->SetNumber(x2, true);
    fNumberEntry_y2->SetNumber(y2, true);
    //     fNumberEntry_x1->SetNumber(x1.f64, true);
    //     fNumberEntry_y1->SetNumber(y1.f64, true);
    //     fNumberEntry_x2->SetNumber(x2.f64, true);
    //     fNumberEntry_y2->SetNumber(y2.f64, true);
}

void MyMainFrame::onSelectedComboBox_mode(int id)
{
    switch (id)
    {
#define MODES_X(mode, txt, classname)       \
    case (Modes::id::mode):                 \
        drawer = &Drawer::draw_##classname; \
        make_all_fields_invisible();        \
        fLabel_k->SetTitle("k");            \
        make_visible_fields_for_##mode();   \
        fMainFrame->Layout();               \
        break;

        MODES
#undef MODES_X

    default:
        break;
    }
}

void MyMainFrame::make_all_fields_invisible()
{
    // Hide all labels
    fLabel_I->UnmapWindow();
    fLabel_I1->UnmapWindow();
    fLabel_I2->UnmapWindow();
    fLabel_k1->UnmapWindow();
    fLabel_k2->UnmapWindow();
    fLabel_k->UnmapWindow();
    fLabel_k_2->UnmapWindow();
    fLabel_k_3->UnmapWindow();
    fLabel_eps->UnmapWindow();
    fLabel_eps2->UnmapWindow();
    fLabel_pass->UnmapWindow();
    fLabel_process->UnmapWindow();
    fLabel_minx1->UnmapWindow();
    fLabel_miny1->UnmapWindow();
    fLabel_maxx1->UnmapWindow();
    fLabel_maxy1->UnmapWindow();
    fLabel_nx1->UnmapWindow();
    fLabel_ny1->UnmapWindow();
    fLabel_onWitch->UnmapWindow();
    fLabel_mineps->UnmapWindow();
    fLabel_maxeps->UnmapWindow();
    fLabel_neps->UnmapWindow();

    // Hide all number entries
    fNumberEntry_I->UnmapWindow();
    fNumberEntry_I1->UnmapWindow();
    fNumberEntry_I2->UnmapWindow();
    fNumberEntry_k1->UnmapWindow();
    fNumberEntry_k2->UnmapWindow();
    fNumberEntry_k->UnmapWindow();
    fNumberEntry_k_2->UnmapWindow();
    fNumberEntry_k_3->UnmapWindow();
    fNumberEntry_eps->UnmapWindow();
    fNumberEntry_eps2->UnmapWindow();
    fNumberEntry_pass->UnmapWindow();
    fNumberEntry_process->UnmapWindow();
    fNumberEntry_minx1->UnmapWindow();
    fNumberEntry_miny1->UnmapWindow();
    fNumberEntry_maxx1->UnmapWindow();
    fNumberEntry_maxy1->UnmapWindow();
    fNumberEntry_nx1->UnmapWindow();
    fNumberEntry_ny1->UnmapWindow();
    fNumberEntry_onWitch->UnmapWindow();
    fNumberEntry_mineps->UnmapWindow();
    fNumberEntry_maxeps->UnmapWindow();
    fNumberEntry_neps->UnmapWindow();

    // Hide checkbox
    fCheckButton_isStachastick->UnmapWindow();
    fCheckButton_connectDots->UnmapWindow();
    fCheckButton_addPortretToBassin->UnmapWindow();
    fCheckButton_drawWithColorPaletteAtRight->UnmapWindow();
    fECanvas->SetBackgroundColor(fECanvas->GetDefaultSelectedBackground());
}

void MyMainFrame::make_visible_fields_for_bifurc()
{
    fLabel_k1->MapWindow();
    fLabel_k2->MapWindow();
    fLabel_I->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();

    fNumberEntry_k1->MapWindow();
    fNumberEntry_k2->MapWindow();
    fNumberEntry_I->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();
}

void MyMainFrame::make_visible_fields_for_bifurcTraction()
{
    fLabel_k1->MapWindow();
    fLabel_k2->MapWindow();
    fLabel_k->MapWindow();
    fLabel_I->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();

    fNumberEntry_k1->MapWindow();
    fNumberEntry_k2->MapWindow();
    fNumberEntry_k->MapWindow();
    fNumberEntry_I->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();

    fLabel_k->SetTitle("k start");
}

void MyMainFrame::make_visible_fields_for_lyapunov()
{
    fLabel_k1->MapWindow();
    fLabel_k2->MapWindow();
    fLabel_I->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();

    fNumberEntry_k1->MapWindow();
    fNumberEntry_k2->MapWindow();
    fNumberEntry_I->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();

    // Add dp
}

void MyMainFrame::make_visible_fields_for_portrait()
{
    fLabel_k->MapWindow();
    fLabel_I->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();
    fLabel_eps->MapWindow();

    fNumberEntry_k->MapWindow();
    fNumberEntry_I->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();
    fNumberEntry_eps->MapWindow();

    fCheckButton_isStachastick->MapWindow();
    fCheckButton_connectDots->MapWindow();
}
void MyMainFrame::make_visible_fields_for_ts()
{
    fLabel_k->MapWindow();
    fLabel_I->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();
    fLabel_eps->MapWindow();

    fNumberEntry_k->MapWindow();
    fNumberEntry_I->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();
    fNumberEntry_eps->MapWindow();

    fCheckButton_isStachastick->MapWindow();
}

void MyMainFrame::make_visible_fields_for_tsTwoEps()
{
    fLabel_k->MapWindow();
    fLabel_I->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();
    fLabel_eps->MapWindow();
    fLabel_eps2->MapWindow();

    fNumberEntry_k->MapWindow();
    fNumberEntry_I->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();
    fNumberEntry_eps->MapWindow();
    fNumberEntry_eps2->MapWindow();

    // fCheckButton_isStachastick->MapWindow();
}

void MyMainFrame::make_visible_fields_for_syncR()
{
    fLabel_k1->MapWindow();
    fLabel_k2->MapWindow();
    fLabel_I->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();
    fLabel_eps->MapWindow();

    fNumberEntry_k1->MapWindow();
    fNumberEntry_k2->MapWindow();
    fNumberEntry_I->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();
    fNumberEntry_eps->MapWindow();

    fCheckButton_isStachastick->MapWindow();
}

void MyMainFrame::make_visible_fields_for_snapshot()
{
    fLabel_I->MapWindow();
    fLabel_k->MapWindow();
    fLabel_minx1->MapWindow();
    fLabel_miny1->MapWindow();
    fLabel_maxx1->MapWindow();
    fLabel_maxy1->MapWindow();
    fLabel_nx1->MapWindow();
    fLabel_ny1->MapWindow();
    fLabel_onWitch->MapWindow();

    fNumberEntry_I->MapWindow();
    fNumberEntry_k->MapWindow();
    fNumberEntry_minx1->MapWindow();
    fNumberEntry_miny1->MapWindow();
    fNumberEntry_maxx1->MapWindow();
    fNumberEntry_maxy1->MapWindow();
    fNumberEntry_nx1->MapWindow();
    fNumberEntry_ny1->MapWindow();
    fNumberEntry_onWitch->MapWindow();

    // fECanvas->SetBackgroundColor(0xff0000);
}

void MyMainFrame::make_visible_fields_for_stochastickLevel()
{
    fLabel_I->MapWindow();
    fLabel_k->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();
    fLabel_neps->MapWindow();
    fLabel_maxeps->MapWindow();
    fLabel_mineps->MapWindow();

    fNumberEntry_I->MapWindow();
    fNumberEntry_k->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();
    fNumberEntry_neps->MapWindow();
    fNumberEntry_maxeps->MapWindow();
    fNumberEntry_mineps->MapWindow();
}

void MyMainFrame::make_visible_fields_for_syncR2D()
{
    fLabel_I1->MapWindow();
    fLabel_I2->MapWindow();
    fLabel_k1->MapWindow();
    fLabel_k2->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();

    fNumberEntry_I1->MapWindow();
    fNumberEntry_I2->MapWindow();
    fNumberEntry_k1->MapWindow();
    fNumberEntry_k2->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();
}

void MyMainFrame::make_visible_fields_for_Lyapunov2D()
{
    fLabel_I1->MapWindow();
    fLabel_I2->MapWindow();
    fLabel_k1->MapWindow();
    fLabel_k2->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();

    fNumberEntry_I1->MapWindow();
    fNumberEntry_I2->MapWindow();
    fNumberEntry_k1->MapWindow();
    fNumberEntry_k2->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();
}

void MyMainFrame::make_visible_fields_for_transientBasin()
{
    fLabel_I->MapWindow();
    fLabel_k->MapWindow();
    fLabel_minx1->MapWindow();
    fLabel_maxx1->MapWindow();
    fLabel_miny1->MapWindow();
    fLabel_maxy1->MapWindow();
    fLabel_nx1->MapWindow();
    fLabel_ny1->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();

    fNumberEntry_I->MapWindow();
    fNumberEntry_k->MapWindow();
    fNumberEntry_minx1->MapWindow();
    fNumberEntry_maxx1->MapWindow();
    fNumberEntry_miny1->MapWindow();
    fNumberEntry_maxy1->MapWindow();
    fNumberEntry_nx1->MapWindow();
    fNumberEntry_ny1->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();

    fCheckButton_addPortretToBassin->MapWindow();
}

void MyMainFrame::make_visible_fields_for_transientBasinColor()
{
    fLabel_I->MapWindow();
    fLabel_k->MapWindow();
    fLabel_minx1->MapWindow();
    fLabel_maxx1->MapWindow();
    fLabel_miny1->MapWindow();
    fLabel_maxy1->MapWindow();
    fLabel_nx1->MapWindow();
    fLabel_ny1->MapWindow();
    fLabel_process->MapWindow();

    fNumberEntry_I->MapWindow();
    fNumberEntry_k->MapWindow();
    fNumberEntry_minx1->MapWindow();
    fNumberEntry_maxx1->MapWindow();
    fNumberEntry_miny1->MapWindow();
    fNumberEntry_maxy1->MapWindow();
    fNumberEntry_nx1->MapWindow();
    fNumberEntry_ny1->MapWindow();
    fNumberEntry_process->MapWindow();

    fCheckButton_drawWithColorPaletteAtRight->MapWindow();
}

void MyMainFrame::make_visible_fields_for_bifurcI()
{
    fLabel_k->MapWindow();
    fLabel_I1->MapWindow();
    fLabel_I2->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();

    fNumberEntry_k->MapWindow();
    fNumberEntry_I1->MapWindow();
    fNumberEntry_I2->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();
}

void MyMainFrame::make_visible_fields_for_Lyapunov2DChaos()
{
    fLabel_I->MapWindow();
    fLabel_k1->MapWindow();
    fLabel_k2->MapWindow();
    fLabel_mineps->MapWindow();
    fLabel_maxeps->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();

    fNumberEntry_I->MapWindow();
    fNumberEntry_k1->MapWindow();
    fNumberEntry_k2->MapWindow();
    fNumberEntry_mineps->MapWindow();
    fNumberEntry_maxeps->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();
}

void MyMainFrame::make_visible_fields_for_syncR2DChaos()
{
    fLabel_I->MapWindow();
    fLabel_k1->MapWindow();
    fLabel_k2->MapWindow();
    fLabel_mineps->MapWindow();
    fLabel_maxeps->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();

    fNumberEntry_I->MapWindow();
    fNumberEntry_k1->MapWindow();
    fNumberEntry_k2->MapWindow();
    fNumberEntry_mineps->MapWindow();
    fNumberEntry_maxeps->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();
}

void MyMainFrame::make_visible_fields_for_LyapunovEps()
{
    fLabel_I->MapWindow();
    fLabel_k->MapWindow();
    fLabel_mineps->MapWindow();
    fLabel_maxeps->MapWindow();
    fLabel_pass->MapWindow();
    fLabel_process->MapWindow();

    fNumberEntry_I->MapWindow();
    fNumberEntry_k->MapWindow();
    fNumberEntry_mineps->MapWindow();
    fNumberEntry_maxeps->MapWindow();
    fNumberEntry_pass->MapWindow();
    fNumberEntry_process->MapWindow();
}

void MyMainFrame::make_visible_fields_for_StabilityOfChaos()
{
    fLabel_I->MapWindow();
    fLabel_k->MapWindow();
    fLabel_k_2->MapWindow();
    fLabel_k_3->MapWindow();
    fLabel_mineps->MapWindow();
    fLabel_maxeps->MapWindow();
    fLabel_process->MapWindow();
    fLabel_neps->MapWindow();

    fNumberEntry_I->MapWindow();
    fNumberEntry_k->MapWindow();
    fNumberEntry_k_2->MapWindow();
    fNumberEntry_k_3->MapWindow();
    fNumberEntry_mineps->MapWindow();
    fNumberEntry_maxeps->MapWindow();
    fNumberEntry_process->MapWindow();
    fNumberEntry_neps->MapWindow();
}