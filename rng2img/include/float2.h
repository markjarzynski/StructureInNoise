#ifndef FLOAT2_H
#define FLOAT2_H

#include <math.h>

class float2
{
public:
    float x, y;

public:
    float2 () {
        x = y = 0.0;
    }

    float2 (float i) {
        x = y = i;
    }

    float2 (float x, float y) {
        this->x = x; this->y = y;
    }

    float2 (float2* v) {
        x = v->x; y = v->y;
    }

    float2 (uint2& v) {
        x = v.x; y = v.y;
    }

};

// vector addition
inline float2 operator + (const float2 &a, const float2 &b)
{
    return float2(a.x + b.x, a.y + b.y);
}

// vector subtraction
inline float2 operator - (const float2 &a, const float2 &b)
{
    return float2(a.x - b.x, a.y - b.y);
}

// scalar addition
inline float2 operator + (const float2 &a, float s)
{
    return float2(a.x + s, a.y + s);
}

inline float2 operator + (float s, const float2 &a)
{
    return float2(s + a.x, s + a.y);
}

// scalar subtraction
inline float2 operator - (const float2 &a, float s)
{
    return float2(a.x - s, a.y - s);
}

inline float2 operator - (float s, const float2 &a)
{
    return float2(s - a.x, s - a.y);
}

// vector multiplication
inline float2 operator * (const float2 &a, const float2 &b)
{
    return float2(a.x * b.x, a.y * b.y);
}

// vector division
inline float2 operator / (const float2 &a, const float2 &b)
{
    return float2(a.x / b.x, a.y / b.y);
}

// scalar multiplication
inline float2 operator * (const float2 &a, float s)
{
    return float2(a.x * s, a.y * s);
}

inline float2 operator * (float s, const float2 &a)
{
    return float2(s * a.x, s * a.y);
}

// scalar division
inline float2 operator / (const float2 &a, float s)
{
    return float2(a.x / s, a.y / s);
}

inline float2 operator / (const float s, const float2 &a)
{
    return float2(s / a.x, s / a.y);
}

inline float dot (const float2 &a, const float2 &b)
{
    return a.x * b.x + a.y * b.y;
}

#endif
