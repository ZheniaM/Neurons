#pragma once

#include <type_traits>

#define has_field_of_type(name, type)                                                      \
    template <class T, typename = void>                                                    \
    struct has_##name##_of_##type : std::false_type                                        \
    {                                                                                      \
    };                                                                                     \
    template <class T>                                                                     \
    struct has_##name##_of_##type<T, std::void_t<decltype(T::name)>>                       \
        : std::is_same<std::remove_cv_t<std::remove_reference_t<decltype(T::name)>>, type> \
    {                                                                                      \
    };
