g++ -o main main.cpp \
    include/funcs.cpp \
    libs/settings.so \
    libs/SystemOf2NeuronsState.so \
    `root-config --cflags --libs`