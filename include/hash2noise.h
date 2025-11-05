#pragma once

#include "hash2rgb.h"
#include "float2.h"
#include "uint2.h"
#include "util.h"

float2 hash2(char* hashname, uint2 v)
{
    v = (uint2(hash2rgb(hashname, v.x, v.y)) >> uint2(13,19)) & uint2(1,1);

    return float2(v.x, v.y) * 2.f - 1.f;
}

float noise2(char* hashname, float2 v)
{
    uint2 vi = uint2(v.x, v.y);
    float2 vf = frac(v);

    float2 t = smoothstep(0.f, 1.f, vf);

    float n00 = dot(hash2(hashname, vi + uint2(0,0)), vf - float2(0,0));
    float n01 = dot(hash2(hashname, vi + uint2(1,0)), vf - float2(1,0));
    float n10 = dot(hash2(hashname, vi + uint2(0,1)), vf - float2(0,1));
    float n11 = dot(hash2(hashname, vi + uint2(1,1)), vf - float2(1,1));

    float n0 = lerp(n00, n10, t.y);
    float n1 = lerp(n01, n11, t.y);

    return lerp(n0, n1, t.x);
}
