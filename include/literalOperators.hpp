#pragma once

#include <string>
#include <stdint.h>

std::string operator""_s(char const *str, std::size_t size) { return std::string(str); }

// union hex2double
// {
//     uint64_t u64;
//     long double f64;
// };

// hex2double operator""_h2b(long double dbl) { return hex2double{.f64 = dbl}; }
// hex2double operator""_h2b(long long i) { return hex2double{.u64 = i}; }