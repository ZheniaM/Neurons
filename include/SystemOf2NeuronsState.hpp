#pragma once

#include <sstream>
#include <iomanip>

#include <stdint.h>
#include <math.h>
#include <random>
#include "Point2x2.hpp"
#include "field_checker.hpp"

struct SystemOf2NeuronsState : Point2x2
{
    template <class T,
              typename = std::enable_if<
                  std::is_same_v<std::remove_cv_t<std::remove_reference_t<decltype(T::P0)>>, Point2x2> &&
                  std::is_same_v<std::remove_const_t<decltype(T::a)>, double> &&
                  std::is_same_v<std::remove_const_t<decltype(T::b)>, double> &&
                  std::is_same_v<std::remove_const_t<decltype(T::c)>, double> &&
                  std::is_same_v<std::remove_const_t<decltype(T::I)>, double> &&
                  std::is_same_v<std::remove_const_t<decltype(T::k)>, double>>>
    SystemOf2NeuronsState(T *t)
        : Point2x2(t->P0), _k(t->k), _I(t->I), a(t->a), b(t->b), c(t->c)
    {
        if constexpr (has_eps_of_double<T>::value)
            _eps = t->eps;

        if constexpr (has_isStochastick_of_bool<T>::value)
            _isStochastick = t->isStochastick;
    }

    SystemOf2NeuronsState(double x1, double y1, double x2, double y2, double k, double I,
                          double a, double b, double c)
        : Point2x2(x1, x2, y1, y2), _k(k), _I(I), _eps(1e-3), a(a), b(b), c(c) {}

    SystemOf2NeuronsState(Point2x2 const &p, double k, double I,
                          double a, double b, double c)
        : Point2x2(p), _k(k), _I(I), _eps(1e-3), a(a), b(b), c(c) {}

    SystemOf2NeuronsState(double x, double y, bool isStachastick, double k, double I);

    uint64_t get_t(void) const { return _t; }
    double get_I(void) const { return _I; }
    double get_k(void) const { return _k; }

    double get_eps(void) const { return _eps; }
    bool get_isStachastick(void) const { return _isStochastick; }

    double next_x1() const;
    double next_y1() const;
    double next_x2() const;
    double next_y2() const;

    Point2x2 next();

    void setIsStochastick(bool const newVal) { _isStochastick = newVal; }
    void setEps(double const eps) { _eps = eps; }

    double phi11(void) const;

    double phi22(void) const;

    void next_rand();

    explicit operator std::string(void) const;

    static constexpr struct
    {
        union
        {
            double lf_x;
            uint64_t u_x;
        };
        union
        {
            double lf_y;
            uint64_t u_y;
        };
    } stablePoint = {.u_x = 0x3fa65a4d262ccbd8, .u_y = 0x4003cac83591c392};

    // struct{double ksi1, double ksi2} get_ksis() { return {ksi1, ksi2}; }
private:
    uint64_t _t = 0;
    const double _k, _I;
    double _eps = 1e-3, _ksi1, _ksi2;
    bool _isStochastick = false;

    double const a = 0.89, b = 0.18, c = 0.28;

    has_field_of_type(eps, double);
    has_field_of_type(isStochastick, bool);
};
