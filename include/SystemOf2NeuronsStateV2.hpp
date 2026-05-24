#pragma once

#include "Point2x2.hpp"
#include "field_checker.hpp"

#include <random>

struct SystemOf2NeuronsStateV2 : Point2x2
{
    template <class T,
              typename = std::enable_if<
                  std::is_same_v<std::remove_cv_t<std::remove_reference_t<decltype(T::P0)>>, Point2x2> &&
                  std::is_same_v<std::remove_const_t<decltype(T::a)>, double> &&
                  std::is_same_v<std::remove_const_t<decltype(T::b)>, double> &&
                  std::is_same_v<std::remove_const_t<decltype(T::c)>, double> &&
                  std::is_same_v<std::remove_const_t<decltype(T::I)>, double> &&
                  std::is_same_v<std::remove_const_t<decltype(T::k)>, double>>>
    SystemOf2NeuronsStateV2(T *t)
        : Point2x2(t->P0), _k(t->k), _I(t->I),
          _a(t->a), _b(t->b), _c(t->c)
    {
        if constexpr (has_eps_of_double<T>::value)
        {
            _eps = t->eps;
        }

        if constexpr (has_isStochastick_of_bool<T>::value)
        {
            _isStochastick = t->isStochastick;
        }
    }

    Point2x2 &next(void);
    explicit operator std::string(void) const;

private:
    double const _a = 0.89, _b = 0.18, _c = 0.28;
    double const _k = 0.0, _I = 0.022;
    double _ksi1 = 0.0, _ksi2 = 0.0;
    double _eps = 0.0;
    bool _isStochastick = false;

    has_field_of_type(eps, double);
    has_field_of_type(isStochastick, bool);
};
