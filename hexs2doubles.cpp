#include <iostream>
#include <stdint.h>

union hexs2doubles
{
    uint64_t u64;
    double dbl;
};

int main(int argc, char *argv[])
{
    hexs2doubles x1, y1, x2, y2;

    // printf("%s\n", argv[1]);
    sscanf(argv[1], "(%lx, %lx, %lx, %lx)", &x1.u64, &y1.u64, &x2.u64, &y2.u64);

    printf("(%lf, %lf, %lf, %lf)\n", x1.dbl, y1.dbl, x2.dbl, y2.dbl);
    printf("(%.10le, %.10le, %.10le, %.10le)\n", x1.dbl, y1.dbl, x2.dbl, y2.dbl);
}