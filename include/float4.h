#ifndef FLOAT4_H
#define FLOAT4_H

#include <math.h>

class float4
{
public:
    float x, y, z, w;

public:
    float4 () {
        x = y = z = w = 0;
    }

    float4 (float i) {
        x = y = z = w = i;
    }

    float4 (float x, float y, float z, float w) {
        this->x = x; this->y = y; this->z = z, this->w = w;
    }

    float4 (float4* v) {
        x = v->x; y = v->y; z = v->z; w = v->w;
    }

    float4 (uint4& v) {
        x = v.x; y = v.y; z = v.z; w = v.w;
    }

public:
    float4& operator += (const float4* rhs)
    {
        x += rhs->x;
        y += rhs->y;
        z += rhs->z;
        w += rhs->w;

        return *this;
    }

    float4& operator += (const float rhs)
    {
        x += rhs;
        y += rhs;
        z += rhs;
        w += rhs;

        return *this;
    }
};

// vector addition
inline float4 operator + (const float4 &a, const float4 &b)
{
    return float4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

// vector subtraction
inline float4 operator - (const float4 &a, const float4 &b)
{
    return float4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

// scalar addition
inline float4 operator + (const float4 &a, float s)
{
    return float4(a.x + s, a.y + s, a.z + s, a.w + s);
}

inline float4 operator + (float s, const float4 &a)
{
    return float4(s + a.x, s + a.y, s + a.z, s + a.w);
}

// scalar subtraction
inline float4 operator - (const float4 &a, float s)
{
    return float4(a.x - s, a.y - s, a.z - s, a.w - s);
}

inline float4 operator - (float s, const float4 &a)
{
    return float4(s - a.x, s - a.y, s - a.z, s - a.w);
}

// vector multiplication
inline float4 operator * (const float4 &a, const float4 &b)
{
    return float4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

// vector division
inline float4 operator / (const float4 &a, const float4 &b)
{
    return float4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

// scalar multiplication
inline float4 operator * (const float4 &a, float s)
{
    return float4(a.x * s, a.y * s, a.z * s, a.w * s);
}

inline float4 operator * (float s, const float4 &a)
{
    return float4(s * a.x, s * a.y, s * a.z, s * a.w);
}

// scalar division
inline float4 operator / (const float4 &a, float s)
{
    return float4(a.x / s, a.y / s, a.z / s, a.w / s);
}

inline float4 operator / (const float s, const float4 &a)
{
    return float4(s / a.x, s / a.y, s / a.z, s / a.w);
}

inline float dot (const float4 &a, const float4 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

#endif
