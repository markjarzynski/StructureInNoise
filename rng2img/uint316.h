#ifndef UINT3_H
#define UINT3_H

#include <cstdint>
#include <math.h>

#include "uint.h"

class uint316
{
public:
    uint16_t x, y, z;

public:
    uint316 () {
        x = y = z = 0;
    }

    uint316 (uint16_t i) {
        x = y = z = i;
    }

    uint316 (uint16_t x, uint16_t y, uint16_t z) {
        this->x = x; this->y = y; this->z = z;
    }
    
    uint316 (uint316* v) {
        x = v->x; y = v->y; z = v->z;
    }

};

// vector addition
inline uint316 operator + (const uint316 &a, const uint316 &b)
{
    return uint316(a.x + b.x, a.y + b.y, a.z + b.z);
}

// vector subtraction
inline uint316 operator - (const uint316 &a, const uint316 &b)
{
    return uint316(a.x - b.x, a.y - b.y, a.z - b.z);
}

// scalar addition
inline uint316 operator + (const uint316 &a, uint16_t s)
{
    return uint316(a.x + s, a.y + s, a.z + s);
}

inline uint316 operator + (uint16_t s, const uint316 &a)
{
    return uint316(s + a.x, s + a.y, s + a.z);
}

// scalar subtraction
inline uint316 operator - (const uint316 &a, uint16_t s)
{
    return uint316(a.x - s, a.y - s, a.z - s);
}

inline uint316 operator - (uint16_t s, const uint316 &a)
{
    return uint316(s - a.x, s - a.y, s - a.z);
}

// vector multiplication
inline uint316 operator * (const uint316 &a, const uint316 &b)
{
    return uint316(a.x * b.x, a.y * b.y, a.z * b.z);
}

// vector division
inline uint316 operator / (const uint316 &a, const uint316 &b)
{
    return uint316(a.x / b.x, a.y / b.y, a.z / b.z);
}

// scalar multiplication
inline uint316 operator * (const uint316 &a, uint16_t s)
{
    return uint316(a.x * s, a.y * s, a.z * s);
}

inline uint316 operator * (uint16_t s, const uint316 &a)
{
    return uint316(s * a.x, s * a.y, s * a.z);
}

// scalar division
inline uint316 operator / (const uint316 &a, uint16_t s)
{
    return uint316(a.x / s, a.y / s, a.z / s);
}

inline uint316 operator / (const uint16_t s, const uint316 &a)
{
    return uint316(s / a.x, s / a.y, s / a.z);
}

inline uint16_t dot (const uint316 &a, const uint316 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline uint316 operator & (const uint316 &a, const uint16_t s)
{
    return uint316(a.x & s, a.y & s, a.z & s);
}

inline uint316 operator >> (const uint316 &a, const uint16_t s)
{
    return uint316(a.x >> s, a.y >> s, a.z >> s);
}

inline uint316 operator ^ (const uint316 &a, const uint16_t s)
{
    return uint316(a.x ^ s, a.y ^ s, a.z ^ s);
}

inline uint316 operator ^ (const uint316 &a, const uint316 &b)
{
    return uint316(a.x ^ b.x, a.y ^ b.y, a.z ^ b.z);
}

#endif
