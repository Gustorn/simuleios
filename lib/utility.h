#ifndef SIMULEIOS_UTILITY_H
#define SIMULEIOS_UTILITY_H

#include <cstddef>

constexpr inline size_t compile_max(size_t x, size_t y) {
  return x > y ? x : y;
}

#endif
