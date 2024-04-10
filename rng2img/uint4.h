#pragma once

#include <cstdint>
#include <math.h>

#include "uint.h"

class uint4
{
public:
    uint x, y, z, w;

public:
    uint4 () {
        x = y = z = w = 0;
    }

    uint4 (uint i) {
        x = y = z = w = i;
    }

    uint4 (uint x, uint y, uint z, uint w) {
        this->x = x; this->y = y; this->z = z, this->w = w;
    }

    uint4 (uint4* v) {
        x = v->x; y = v->y; z = v->z; w = v->w;
    }
};

// vector addition
inline uint4 operator + (const uint4 &a, const uint4 &b)
{
    return uint4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

// vector subtraction
inline uint4 operator - (const uint4 &a, const uint4 &b)
{
    return uint4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

// scalar addition
inline uint4 operator + (const uint4 &a, uint s)
{
    return uint4(a.x + s, a.y + s, a.z + s, a.w + s);
}

inline uint4 operator + (uint s, const uint4 &a)
{
    return uint4(s + a.x, s + a.y, s + a.z, s + a.w);
}

// scalar subtraction
inline uint4 operator - (const uint4 &a, uint s)
{
    return uint4(a.x - s, a.y - s, a.z - s, a.w - s);
}

inline uint4 operator - (uint s, const uint4 &a)
{
    return uint4(s - a.x, s - a.y, s - a.z, s - a.w);
}

// vector multiplication
inline uint4 operator * (const uint4 &a, const uint4 &b)
{
    return uint4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

// vector division
inline uint4 operator / (const uint4 &a, const uint4 &b)
{
    return uint4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

// scalar multiplication
inline uint4 operator * (const uint4 &a, uint s)
{
    return uint4(a.x * s, a.y * s, a.z * s, a.w * s);
}

inline uint4 operator * (uint s, const uint4 &a)
{
    return uint4(s * a.x, s * a.y, s * a.z, s * a.w);
}

// scalar division
inline uint4 operator / (const uint4 &a, uint s)
{
    return uint4(a.x / s, a.y / s, a.z / s, a.w / s);
}

inline uint4 operator / (const uint s, const uint4 &a)
{
    return uint4(s / a.x, s / a.y, s / a.z, s / a.w);
}

inline uint dot (const uint4 &a, const uint4 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline uint4 operator >> (const uint4 &a, const uint s)
{
    return uint4(a.x >> s, a.y >> s, a.z >> s, a.w >> s);
}

inline uint4 operator ^ (const uint4 &a, const uint4 &b)
{
    return uint4(a.x ^ b.x, a.y ^ b.y, a.z ^ b.z, a.w ^ b.w);
}
