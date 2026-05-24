rootcling -f MyMainFrameDict.cpp -c \
    MyMainFrame.hpp MyMainFrameLinkDef.h

# rm MyMainFrameDict*_dict*
# rm MyMainFrameDict_*


ROOT_FLAGS=$(root-config --cflags --glibs)
CC=g++
# CC=g++-mingw-w64

$CC -o MyMainFrame main_gui.cpp \
    include/funcs.cpp \
    include/Point2x2.cpp \
    include/neuron_funcs/Drawer.cpp \
    include/neuron_funcs/Bifurc.cpp \
    include/neuron_funcs/BifurcI.cpp \
    include/neuron_funcs/BifurcTraction.cpp \
    include/neuron_funcs/Portrait.cpp \
    include/neuron_funcs/SyncR.cpp \
    include/neuron_funcs/SyncR2D.cpp \
    include/neuron_funcs/SyncR2DChaos.cpp \
    include/neuron_funcs/TimeSiries.cpp \
    include/neuron_funcs/Lyapunov.cpp \
    include/neuron_funcs/Lyapunov2D.cpp \
    include/neuron_funcs/Lyapunov2DChaos.cpp \
    include/neuron_funcs/LyapunovEps.cpp \
    include/neuron_funcs/Snapshot.cpp \
    include/neuron_funcs/StochastickLevel.cpp \
    include/neuron_funcs/TransientBasin.cpp \
    include/neuron_funcs/StabilityOfChaos.cpp \
    include/SystemOf2NeuronsState.cpp \
    include/SystemOf2NeuronsStateV2.cpp \
    MyMainFrame.cpp \
    MyMainFrameDict.cpp \
    $ROOT_FLAGS



    # libs/system_of_2_neurons_state.so.orig.so \
    # libs/settings.so \