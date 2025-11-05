#ifndef UTIL_H
#define UTIL_H

#include "float2.h"
#include "float3.h"
#include "float4.h"

#include <cstring>
#include <math.h>

double frac( double m )
{
    double i;
    return modf(m, &i);
}

float frac( float m )
{
    double i;
    return (float) modf(m, &i);
}

float2 frac( float2 m )
{
    return float2( frac(m.x), frac(m.y) );
}

float3 frac( float3 m )
{
    return float3( frac(m.x), frac(m.y), frac(m.z) );
}

float4 frac( float4 m)
{
    return float4( frac(m.x), frac(m.y), frac(m.z), frac(m.w) );
}

float min(float a, float b)
{
    return a < b ? a : b;
}

float max(float a, float b)
{
    return a > b ? a : b;
}

float2 min(float2 a, float2 b)
{
    return float2(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y);
}

float2 max(float2 a, float2 b)
{
    return float2(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y);
}

float clamp(float x, float minval = 0.f, float maxval = 1.f)
{
    return min(max(x, minval), maxval);
}

float2 clamp(float2 x, float2 minval = {0.f}, float2 maxval = {1.f})
{
    return min(max(x, minval), maxval);
}

float smoothstep(float a, float b, float x)
{
    x = clamp((x - a) / (b - a), 0.f, 1.f);
    return x;

    return x * x * (3.f - 2.f * x);
}

float2 smoothstep(float2 a, float2 b, float2 x)
{
    x = clamp((x - a) / (b - a), 0.f, 1.f);
    return x * x * (3.f - 2.f * x);
}

float lerp(float a, float b, float x)
{
    //return a + x * (b - a);
    return (1.f - x) * a + x * b;
}

float2 lerp(float2 a, float2 b, float2 x)
{
    //return a + x * (b - a);
    return (1.f - x) * a + x * b;
}

#endif
