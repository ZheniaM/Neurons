#include "SystemOf2NeuronsState.hpp"

#include <stdint.h>
#include <math.h>
#include <random>


SystemOf2NeuronsState::SystemOf2NeuronsState(double x, double y, bool isStachastick, double k, double I)
    : Point2x2(x, y, x, y), _isStochastick(isStachastick), _k(k), _I(I)
{
    unsigned const PASS = 1'000;
    double xNext, yNext;
    uint64_t i = 0;

    for (; i < PASS / 2; ++i)
    {
        next_rand();
        xNext = x * x * exp(y - x) + _I + (_isStochastick ? (_ksi1 * _eps) : 0);
        yNext = a * y - b * x + c;
        x = xNext, y = yNext;
    }
    _x1 = x, _y1 = y;

    for (; i < PASS; ++i)
    {
        next_rand();
        xNext = x * x * exp(y - x) + _I + (_isStochastick ? (_ksi1 * _eps) : 0);
        yNext = a * y - b * x + c;
        x = xNext, y = yNext;
    }
    _x2 = x, _y2 = y;
}

void SystemOf2NeuronsState::next_rand(void)
{
    static bool init = false;
    if (!init)
    {
        srand(std::random_device()());
        init = true;
    }
    double r1 = ((double)rand()) / ((double)RAND_MAX);
    double r2 = ((double)rand()) / ((double)RAND_MAX);
    _ksi1 = sqrt(-2 * log(r1)) * cos(2.0 * M_PI * r2);
    _ksi2 = sqrt(-2 * log(r1)) * sin(2.0 * M_PI * r2);
}

double SystemOf2NeuronsState::phi11() const
{
    double nonstach = _x1 * _x1 * std::exp(_y1 - _x1) + _I;
    double stach = 0.0;
    if (_isStochastick)
        stach = _eps * _ksi1;
    return nonstach + stach;
}

double SystemOf2NeuronsState::phi22() const
{
    double nonstach = _x2 * _x2 * std::exp(_y2 - _x2) + _I;
    double stach = 0.0;
    if (_isStochastick)
        stach = _eps * _ksi2;
    return nonstach + stach;
}

double SystemOf2NeuronsState::next_y1() const
{
    return a * _y1 - b * _x1 + c;
}

double SystemOf2NeuronsState::next_y2() const
{
    return a * _y2 - b * _x2 + c;
}

Point2x2 SystemOf2NeuronsState::next()
{
    next_rand();
    double x1 = next_x1();
    double y1 = next_y1();
    double x2 = next_x2();
    double y2 = next_y2();
    _x1 = x1;
    _x2 = x2;
    _y1 = y1;
    _y2 = y2;
    ++_t;
    return *this;
}

// #define ITALIAN_VARIANT
#ifndef ITALIAN_VARIANT

double SystemOf2NeuronsState::next_x1() const
{
    return phi11() + _k * (_x2 - _x1);
}

double SystemOf2NeuronsState::next_x2() const
{
    return phi22() + _k * (_x1 - _x2);
}

#else /// ITALIAN VARIANT

double SystemOf2NeuronsState::next_x1()
{
    double k = _k;
    double phi11 = this->phi11();
    double phi22 = this->phi22();
    return (phi11 + (k / (1.0 + k)) * phi22) / (1.0 + k - k * k / (1.0 + k));
}

double SystemOf2NeuronsState::next_x2()
{
    double k = _k;
    double phi11 = this->phi11();
    double phi22 = this->phi22();
    return (phi22 + (k / (1.0 + k)) * phi11) / (1.0 + k - k * k / (1.0 + k));
}

#endif /// ITALIAN VARIANT END

SystemOf2NeuronsState::operator std::string(void) const
{
    std::stringstream ss;
    auto add = [&ss](double val) -> std::ostream &
    {
        ss << std::hex << std::setfill('0') << std::setw(16) << reinterpret_cast<int64_t const &>(val);
        return ss;
    };
    ss << "point: " << static_cast<std::string>(static_cast<Point2x2 const>(*this));
    ss << ", a: 0x", add(a);
    ss << ", b: 0x", add(b);
    ss << ", c: 0x", add(c);

    ss << ", k: 0x", add(_k);
    ss << ", I: 0x", add(_I);

    ss << ", isStochastick: 0x", add(_isStochastick);
    ss << ", eps: 0x", add(_eps);
    return ss.str();
}