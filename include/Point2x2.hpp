#pragma once

#include <string>

#define printPoint2x2(p) printf(#p": %s\n", static_cast<std::string>(p).c_str());

struct Point2x2
{
    Point2x2(double x1, double y1, double x2, double y2)
        : _x1(x1), _y1(y1), _x2(x2), _y2(y2) {}

    Point2x2(Point2x2 const &p)
        : _x1(p._x1), _y1(p._y1), _x2(p._x2), _y2(p._y2) {}

    Point2x2(Point2x2 &&p)
        : _x1(p._x1), _y1(p._y1), _x2(p._x2), _y2(p._y2) {}

    Point2x2(uint64_t x12, uint64_t y12)
        : _x1(*reinterpret_cast<double *>(&x12)),
          _y1(*reinterpret_cast<double *>(&y12)),
          _x2(*reinterpret_cast<double *>(&x12)),
          _y2(*reinterpret_cast<double *>(&y12)) {}

    Point2x2 &operator=(Point2x2 const &other);

    ~Point2x2() = default;

    double get_x1() const { return _x1; }
    double get_x2() const { return _x2; }
    double get_y1() const { return _y1; }
    double get_y2() const { return _y2; }

    explicit operator std::string(void) const;

    Point2x2 operator+(Point2x2 const &p) const { return Point2x2(_x1 + p.get_x1(), _y1 + p.get_y1(), _x2 + p.get_x2(), _y2 + p.get_y2()); }
    Point2x2 operator-(Point2x2 const &p) const { return Point2x2(_x1 - p.get_x1(), _y1 - p.get_y1(), _x2 - p.get_x2(), _y2 - p.get_y2()); }
    Point2x2 operator*(double a) const { return Point2x2{_x1 * a, _y1 * a, _x2 * a, _y2 * a}; }
    Point2x2 operator/(double a) const { return Point2x2{_x1 / a, _y1 / a, _x2 / a, _y2 / a}; }

    Point2x2 normalized(void) const;
    double mod(void) const;

protected:
    double _x1, _y1, _x2, _y2;
};