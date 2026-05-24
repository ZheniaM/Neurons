#include "Point2x2.hpp"

#include <sstream>
#include <cmath>

// union hex2double
// {
//     uint64_t u64;
//     double f64;
// };

// constexpr Point2x2::Point2x2(uint64_t x12, uint64_t y12)
// {
//     _x1 = hex2double{.u64 = x12}.f64;
//     _x2 = hex2double{.u64 = x12}.f64;
//     _y1 = hex2double{.u64 = y12}.f64;
//     _y2 = hex2double{.u64 = y12}.f64;
// }
Point2x2 &Point2x2::operator=(Point2x2 const &p)
{
    if (this == &p)
        return *this;
    // printf("op=\n");
    _x1 = p._x1;
    _y1 = p._y1;
    _x2 = p._x2;
    _y2 = p._y2;
    return *this;
}

Point2x2::operator std::string(void) const
{
    union U
    {
        uint64_t i;
        double d;
    };
    // U u{.d = _x1};
    // U{.d = 1.0}.i;
    // printf("%lf, %lx ", u.d, u.i);
    std::stringstream ss;
    ss << "(x1, y1, x2, y2) = (" << _x1 << ", " << _y1 << ", " << _x2 << ", " << _y2 << ") =";
    ss << " (" << std::hex << U{.d = _x1}.i;
    ss << ", " << std::hex << U{.d = _y1}.i;
    ss << ", " << std::hex << U{.d = _x2}.i;
    ss << ", " << std::hex << U{.d = _y2}.i << ")";
    return ss.str();
}

double Point2x2::mod(void) const
{
    return std::sqrt(_x1 * _x1 + _y1 * _y1 + _x2 * _x2 + _y2 * _y2);
}

Point2x2 Point2x2::normalized(void) const
{
    double m = mod();
    return Point2x2(_x1 / m, _y1 / m, _x2 / m, _y2 / m);
}