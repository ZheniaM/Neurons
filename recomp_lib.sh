if [ ! -d libs ]
then
    mkdir libs
fi

g++ -shared -fPIC -o libs/$1.so include/$1.cpp include/funcs.cpp `root-config --cflags --libs`