#pragma once

#include <cstdint>
#include <math.h>

#include "uint.h"

class uint2
{
public:
    uint x, y;

public:
    uint2 () {
        x = y = 0;
    }

    uint2 (uint i) {
        x = y = i;
    }

    uint2 (uint x, uint y) {
        this->x = x; this->y = y;
    }

    uint2 (uint2* v) {
        x = v->x; y = v->y;
    }

    ~uint2(){}

};

// vector addition
inline uint2 operator + (const uint2 &a, const uint2 &b)
{
    return uint2(a.x + b.x, a.y + b.y);
}

// vector subtraction
inline uint2 operator - (const uint2 &a, const uint2 &b)
{
    return uint2(a.x - b.x, a.y - b.y);
}

// scalar addition
inline uint2 operator + (const uint2 &a, uint s)
{
    return uint2(a.x + s, a.y + s);
}

inline uint2 operator + (uint s, const uint2 &a)
{
    return uint2(s + a.x, s + a.y);
}

// scalar subtraction
inline uint2 operator - (const uint2 &a, uint s)
{
    return uint2(a.x - s, a.y - s);
}

inline uint2 operator - (uint s, const uint2 &a)
{
    return uint2(s - a.x, s - a.y);
}

// vector multiplication
inline uint2 operator * (const uint2 &a, const uint2 &b)
{
    return uint2(a.x * b.x, a.y * b.y);
}

// vector division
inline uint2 operator / (const uint2 &a, const uint2 &b)
{
    return uint2(a.x / b.x, a.y / b.y);
}

// scalar multiplication
inline uint2 operator * (const uint2 &a, uint s)
{
    return uint2(a.x * s, a.y * s);
}

inline uint2 operator * (uint s, const uint2 &a)
{
    return uint2(s * a.x, s * a.y);
}

// scalar division
inline uint2 operator / (const uint2 &a, uint s)
{
    return uint2(a.x / s, a.y / s);
}

inline uint2 operator / (const uint s, const uint2 &a)
{
    return uint2(s / a.x, s / a.y);
}

inline uint dot (const uint2 &a, const uint2 &b)
{
    return a.x * b.x + a.y * b.y;
}

inline uint2 operator >> (const uint2 &a, const uint s)
{
    return uint2(a.x >> s, a.y >> s);
}

inline uint2 operator >> (const uint2 &a, const uint2 &b)
{
    return uint2(a.x >> b.x, a.y >> b.y);
}

inline uint2 operator ^ (const uint2 &a, const uint s)
{
    return uint2(a.x ^ s, a.y ^ s);
}

inline uint2 operator ^ (const uint2 &a, const uint2 &b)
{
    return uint2(a.x ^ b.x, a.y ^ b.y);
}
