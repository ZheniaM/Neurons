#include "SystemOf2NeuronsStateV2.hpp"
#include <sstream>
#include <string>
#include <iomanip>

Point2x2 &SystemOf2NeuronsStateV2::next(void)
{
    if (_isStochastick)
    {
        double r1 = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
        double r2 = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
        _ksi1 = sqrt(-2 * std::log(r1)) * std::cos(2.0 * M_PI * r2);
        _ksi2 = sqrt(-2 * std::log(r1)) * std::sin(2.0 * M_PI * r2);

        double x1Next = _x1 * _x1 * std::exp(_y1 - _x1) + _I + _k * (_x2 - _x1) + _eps * _ksi1;
        double x2Next = _x2 * _x2 * std::exp(_y2 - _x2) + _I + _k * (_x1 - _x2) + _eps * _ksi2;
        double y1Next = _a * _y1 - _b * _x1 + _c;
        double y2Next = _a * _y2 - _b * _x2 + _c;

        _x1 = x1Next;
        _y1 = y1Next;
        _x2 = x2Next;
        _y2 = y2Next;
    }
    else
    {
        double x1Next = _x1 * _x1 * std::exp(_y1 - _x1) + _I + _k * (_x2 - _x1);
        double x2Next = _x2 * _x2 * std::exp(_y2 - _x2) + _I + _k * (_x1 - _x2);
        double y1Next = _a * _y1 - _b * _x1 + _c;
        double y2Next = _a * _y2 - _b * _x2 + _c;

        _x1 = x1Next;
        _y1 = y1Next;
        _x2 = x2Next;
        _y2 = y2Next;
    }

    return *this;
}

SystemOf2NeuronsStateV2::operator std::string(void) const
{
    std::stringstream ss;
    auto add = [&ss](double val) -> std::ostream &
    {
        ss << std::hex << std::setfill('0') << std::setw(16) << reinterpret_cast<int64_t const &>(val);
        return ss;
    };
    ss << "point: " << static_cast<std::string>(static_cast<Point2x2 const>(*this));
    ss << ", a: 0x", add(_a);
    ss << ", b: 0x", add(_b);
    ss << ", c: 0x", add(_c);

    ss << ", k: 0x", add(_k);
    ss << ", I: 0x", add(_I);

    ss << ", isStochastick: 0x", add(_isStochastick);
    ss << ", eps: 0x", add(_eps);
    return ss.str();
}
