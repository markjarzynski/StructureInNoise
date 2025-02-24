#pragma once

#include <stdint.h>

void bitshift(uint8_t* out, uint8_t* in, int size, int shift)
{
    for (int i = 0; i < size; i++)
    {
        out[i] = (in[i] >> shift) & 1;
    }
}
