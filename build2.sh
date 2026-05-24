flags=`root-config --glibs --cflags --libs`
INCLUDE_FILES=""
if [ -d "include" ]; then
    # Ищем все .cpp, .c, .o файлы в include и подпапках
    INCLUDE_FILES=$(find include -name "*.cpp" -o -name "*.c" -o -name "*.o" 2>/dev/null)
fi



# echo $INCLUDE_FILES
g++ --std=c++11 -I`root-config --incdir` bif2_dict.cpp -o $1 $1.cpp $INCLUDE_FILES $flags