#pragma once

#include <stdint.h>

// https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
uint32_t popcnt(uint32_t v)
{

    v = v - ((v >> 1) & 0x55555555);
    v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
    return ((v + (v >> 4) & 0x0F0F0F0F) * 0x01010101) >> 24;
}