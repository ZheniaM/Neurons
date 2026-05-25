#pragma once

#include <stdint.h>

#define MODES                                                                  \
    MODES_X(bifurc, "Bifurcation k", Bifurc)                                   \
    MODES_X(bifurcI, "Bifurcation I", BifurcI)                                 \
    MODES_X(bifurcTraction, "Bifurcation by traction", BifurcTraction)         \
    MODES_X(ts, "Time series", TimeSiries)                                     \
    MODES_X(tsTwoEps, "Time series two eps", TimeSiriesTwoEps)                 \
    MODES_X(portrait, "Portrait", Portrait)                                    \
    MODES_X(snapshot, "Snapshot", Snapshot)                                    \
    MODES_X(stochastickLevel, "Stochastick level", StochastickLevel)           \
    MODES_X(transientBasin, "Transient basin", TransientBasin)                 \
    MODES_X(transientBasinColor, "Transient basin color", TransientBasinColor) \
    MODES_X(syncR, "Sync R", SyncR)                                            \
    MODES_X(syncR2D, "Sync R 2D", SyncR2D)                                     \
    MODES_X(syncR2DChaos, "Sync R 2D K-EPS", SyncR2DChaos)                     \
    MODES_X(lyapunov, "Lyapunov", Lyapunov)                                    \
    MODES_X(Lyapunov2D, "Lyapunov 2D", Lyapunov2D)                             \
    MODES_X(Lyapunov2DChaos, "Lyapunov 2D K-EPS", Lyapunov2DChaos)             \
    MODES_X(LyapunovEps, "Lyapunov EPS", LyapunovEps)                          \
    MODES_X(StabilityOfChaos, "Stility of StablePoint", StabilityOfChaos)

namespace Modes
{
#define MODES_X(mode, txt, classname) mode,
    enum id : int32_t
    {
        MODES
    };
#undef MODES_X

    // #define MODES_X(mode, txt, classname) txt,
    //     char const *name[] = {MODES};
    // #undef MODES_X
};