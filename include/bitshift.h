#pragma once

#include <stdint.h>

#include "uint3.h"

void bitshift(uint8_t* out, uint8_t* in, int size, int shift)
{
    for (int i = 0; i < size; i++)
    {
        out[i] = (in[i] >> shift) & 1;
    }
}

void bitshift(uint32_t* inout, int size, int shift)
{
    for (int i = 0; i < size; i++)
    {
        inout[i] = (inout[i] >> shift);
    }
}

void rotr(uint32_t* inout, int size, int shift)
{
    for (int i = 0; i < size; i++)
    {
        inout[i] = (inout[i] >> shift) | (inout[i] << (32 - shift));
    }
}

void rotl(uint32_t* inout, int size, int shift)
{
    for (int i = 0; i < size; i++)
    {
        inout[i] = (inout[i] << shift) | (inout[i] >> (32 - shift));
    }
}
