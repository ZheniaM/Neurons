#pragma once

#include "Point2x2.hpp"
#include "field_checker.hpp"

#include <random>

struct SystemOf2NeuronsStateV2InitState
{
    Point2x2 const &P0;
    double const a = 0.89;
    double const b = 0.18;
    double const c = 0.28;
    double const I;
    double const k;
};

struct SystemOf2NeuronsStateV2InitStateChaos
{
    Point2x2 const &P0;
    double const a = 0.89;
    double const b = 0.18;
    double const c = 0.28;
    double const I;
    double const k;
    double const eps;
    bool const isStochastick = true;
};

struct SystemOf2NeuronsStateV2 : Point2x2
{
    SystemOf2NeuronsStateV2(SystemOf2NeuronsStateV2InitState *initState)
        : Point2x2(initState->P0),
          _a(initState->a),
          _b(initState->b),
          _c(initState->c),
          _k(initState->k),
          _I(initState->I)
    {
    }

    SystemOf2NeuronsStateV2(SystemOf2NeuronsStateV2InitStateChaos *initState)
        : Point2x2(initState->P0),
          _a(initState->a),
          _b(initState->b),
          _c(initState->c),
          _k(initState->k),
          _I(initState->I),
          _eps(initState->eps),
          _isStochastick(initState->isStochastick)
    {
    }

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
