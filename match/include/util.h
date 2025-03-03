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

#endif
