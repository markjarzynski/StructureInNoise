#pragma once

#include <cstdint>
#include <math.h>

#include "uint.h"

class uint3
{
public:
    uint x, y, z;

public:
    uint3 () {
        x = y = z = 0;
    }

    uint3 (uint i) {
        x = y = z = i;
    }

    uint3 (uint x, uint y, uint z) {
        this->x = x; this->y = y; this->z = z;
    }
    
    uint3 (uint3* v) {
        x = v->x; y = v->y; z = v->z;
    }

    ~uint3(){}

};

// vector addition
inline uint3 operator + (const uint3 &a, const uint3 &b)
{
    return uint3(a.x + b.x, a.y + b.y, a.z + b.z);
}

// vector subtraction
inline uint3 operator - (const uint3 &a, const uint3 &b)
{
    return uint3(a.x - b.x, a.y - b.y, a.z - b.z);
}

// scalar addition
inline uint3 operator + (const uint3 &a, uint s)
{
    return uint3(a.x + s, a.y + s, a.z + s);
}

inline uint3 operator + (uint s, const uint3 &a)
{
    return uint3(s + a.x, s + a.y, s + a.z);
}

// scalar subtraction
inline uint3 operator - (const uint3 &a, uint s)
{
    return uint3(a.x - s, a.y - s, a.z - s);
}

inline uint3 operator - (uint s, const uint3 &a)
{
    return uint3(s - a.x, s - a.y, s - a.z);
}

// vector multiplication
inline uint3 operator * (const uint3 &a, const uint3 &b)
{
    return uint3(a.x * b.x, a.y * b.y, a.z * b.z);
}

// vector division
inline uint3 operator / (const uint3 &a, const uint3 &b)
{
    return uint3(a.x / b.x, a.y / b.y, a.z / b.z);
}

// scalar multiplication
inline uint3 operator * (const uint3 &a, uint s)
{
    return uint3(a.x * s, a.y * s, a.z * s);
}

inline uint3 operator * (uint s, const uint3 &a)
{
    return uint3(s * a.x, s * a.y, s * a.z);
}

// scalar division
inline uint3 operator / (const uint3 &a, uint s)
{
    return uint3(a.x / s, a.y / s, a.z / s);
}

inline uint3 operator / (const uint s, const uint3 &a)
{
    return uint3(s / a.x, s / a.y, s / a.z);
}

inline uint dot (const uint3 &a, const uint3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline uint3 operator & (const uint3 &a, const uint s)
{
    return uint3(a.x & s, a.y & s, a.z & s);
}

inline uint3 operator >> (const uint3 &a, const uint s)
{
    return uint3(a.x >> s, a.y >> s, a.z >> s);
}

inline uint3 operator << (const uint3 &a, const uint s)
{
    return uint3(a.x << s, a.y << s, a.z << s);
}

inline uint3 operator ^ (const uint3 &a, const uint s)
{
    return uint3(a.x ^ s, a.y ^ s, a.z ^ s);
}

inline uint3 operator ^ (const uint3 &a, const uint3 &b)
{
    return uint3(a.x ^ b.x, a.y ^ b.y, a.z ^ b.z);
}
