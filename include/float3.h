#ifndef FLOAT3_H
#define FLOAT3_H

#include <math.h>

class float3
{
public:
    float x, y, z;

public:
    float3 () {
        x = y = z = 0;
    }

    float3 (float i) {
        x = y = z = i;
    }

    float3 (float x, float y, float z) {
        this->x = x; this->y = y; this->z = z;
    }
    
    float3 (float3* v) {
        x = v->x; y = v->y; z = v->z;
    }

    float3 (uint3& v) {
        x = v.x; y = v.y; z = v.z;
    }

public:
    float3& operator += (const float3* rhs)
    {
        x += rhs->x;
        y += rhs->y;
        z += rhs->z;

        return *this;
    }

    float3& operator += (const float rhs)
    {
        x += rhs;
        y += rhs;
        z += rhs;

        return *this;
    }
};

// vector addition
inline float3 operator + (const float3 &a, const float3 &b)
{
    return float3(a.x + b.x, a.y + b.y, a.z + b.z);
}

// vector subtraction
inline float3 operator - (const float3 &a, const float3 &b)
{
    return float3(a.x - b.x, a.y - b.y, a.z - b.z);
}

// scalar addition
inline float3 operator + (const float3 &a, float s)
{
    return float3(a.x + s, a.y + s, a.z + s);
}

inline float3 operator + (float s, const float3 &a)
{
    return float3(s + a.x, s + a.y, s + a.z);
}

// scalar subtraction
inline float3 operator - (const float3 &a, float s)
{
    return float3(a.x - s, a.y - s, a.z - s);
}

inline float3 operator - (float s, const float3 &a)
{
    return float3(s - a.x, s - a.y, s - a.z);
}

// vector multiplication
inline float3 operator * (const float3 &a, const float3 &b)
{
    return float3(a.x * b.x, a.y * b.y, a.z * b.z);
}

// vector division
inline float3 operator / (const float3 &a, const float3 &b)
{
    return float3(a.x / b.x, a.y / b.y, a.z / b.z);
}

// scalar multiplication
inline float3 operator * (const float3 &a, float s)
{
    return float3(a.x * s, a.y * s, a.z * s);
}

inline float3 operator * (float s, const float3 &a)
{
    return float3(s * a.x, s * a.y, s * a.z);
}

// scalar division
inline float3 operator / (const float3 &a, float s)
{
    return float3(a.x / s, a.y / s, a.z / s);
}

inline float3 operator / (const float s, const float3 &a)
{
    return float3(s / a.x, s / a.y, s / a.z);
}

inline float dot (const float3 &a, const float3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

#endif
