#pragma once

#include <math.h>

#include "uint.h"
#include "uint2.h"
#include "uint3.h"
#include "uint4.h"

#include "float2.h"
#include "float3.h"

#include "uint316.h"

#include "util.h"

// Commonly used constants
#define c1 0xcc9e2d51u
#define c2 0x1b873593u

// Helper Functions
uint rotl(uint x, uint r)
{
	return (x << r) | (x >> (32u - r));   
}

uint rotr(uint x, uint r)   
{
	return (x >> r) | (x << (32u - r));
}

uint fmix(uint h)
{
  h ^= h >> 16;
  h *= 0x85ebca6bu;
  h ^= h >> 13;
  h *= 0xc2b2ae35u;
  h ^= h >> 16;
  return h;
}

uint mur(uint a, uint h) {
  // Helper from Murmur3 for combining two 32-bit values.
  a *= c1;
  a = rotr(a, 17u);
  a *= c2;
  h ^= a;
  h = rotr(h, 19u);
  return h * 5u + 0xe6546b64u;
}

uint bswap32(uint x) {
    return (((x & 0x000000ffu) << 24) |
            ((x & 0x0000ff00u) <<  8) |
            ((x & 0x00ff0000u) >>  8) |
            ((x & 0xff000000u) >> 24));
}

// convert 2D seed to 1D
uint seed(uint2 p) {
    return 19u * p.x + 47u * p.y + 101u;
}

// convert 3D seed to 1D
uint seed3(uint3 p) {
    return 19u * p.x + 47u * p.y + 101u * p.z + 131u;
}

uint seed4(uint4 p) {
    return 19u * p.x + 47u * p.y + 101u * p.z + 131u * p.w + 173u;
}

uint newseed(uint2 p) {
    return p.x * 1664525u + p.y * 16807u + 1013904223u;
}

// Hashes

// BBS-inspired hash
//  - Olano, Modified Noise for Evaluation on Graphics Hardware, GH 2005
uint bbs(uint v)
{
    v = v % 65521u;
    v = (v * v) % 65521u;
    v = (v * v) % 65521u;
    return v;
}

// CityHash32, adapted from Hash32Len0to4 in https://github.com/google/cityhash
uint city(uint s)
{
    uint len = 4u;
	uint b = 0u;
    uint c = 9u;
    
    for (uint i = 0u; i < len; i++) {
    	uint v = (s >> (i * 8u)) & 0xffu;
        b = b * c1 + v;
        c ^= b;
    }
    
    return fmix(mur(b, mur(len, c)));
}

// CityHash32, adapted from Hash32Len5to12 in https://github.com/google/cityhash
uint city(uint2 s)
{
    uint len = 8u;
    uint a = len, b = len * 5, c = 9, d = b;

    a += bswap32(s.x);
    b += bswap32(s.y);
    c += bswap32(s.y);

    return fmix(mur(c, mur(b, mur(a, d)))); 
}

// CityHash32, adapted from Hash32Len5to12 in https://github.com/google/cityhash
uint city(uint3 s)
{
    uint len = 12u;
    uint a = len, b = len * 5, c = 9, d = b;

    a += bswap32(s.x);
    b += bswap32(s.z);
    c += bswap32(s.y);

    return fmix(mur(c, mur(b, mur(a, d)))); 
}

// CityHash32, adapted from Hash32Len12to24 in https://github.com/google/cityhash
uint city(uint4 s)
{
    uint len = 16u;
    uint a = bswap32(s.w);
    uint b = bswap32(s.y);
    uint c = bswap32(s.z);
    uint d = bswap32(s.z);
    uint e = bswap32(s.x);
    uint f = bswap32(s.w);
    uint h = len;

    return fmix(mur(f, mur(e, mur(d, mur(c, mur(b, mur(a, h))))))); 
}

// Hash from https://www.cs.ubc.ca/~rbridson/docs/schechter-sca08-turbulence.pdf
// - Schechter and Bridson, Evolving Sub-Grid Turbulence for Smoke Animation, 2008
uint esgtsa(uint s) {
    s = (s ^ 2747636419u) * 2654435769u;// % 4294967296u;
    s = (s ^ (s >> 16u)) * 2654435769u;// % 4294967296u;
    s = (s ^ (s >> 16u)) * 2654435769u;// % 4294967296u;
    return s;
}

// UE4 RandFast function
float fast(float2 v) {
    v = (1./4320.) * v + float2(0.25,0.);
    float state = frac( dot( v * v, float2(3571)));
    return frac( state * state * (3571. * 2.));
}

// Interleaved Gradient Noise
//  - Jimenez, Next Generation Post Processing in Call of Duty: Advanced Warfare
//    Advances in Real-time Rendering, SIGGRAPH 2014
float ign(float2 v)
{
    float3 magic = float3(0.06711056f, 0.00583715f, 52.9829189f);
    return frac(magic.z * frac(dot(v, float2(magic.x, magic.y))));//frac(magic.z * frac(dot(v, magic.xy)));
}

// Integer Hash - I
// - Inigo Quilez, Integer Hash - I, 2017
//   https://www.shadertoy.com/view/llGSzw
uint iqint1(uint n)
{
    // integer hash copied from Hugo Elias
	n = (n << 13U) ^ n;
    n = n * (n * n * 15731U + 789221U) + 1376312589U; 
    
    return n;
}

// Integer Hash - II
// - Inigo Quilez, Integer Hash - II, 2017
//   https://www.shadertoy.com/view/XlXcW4
uint3 iqint2(uint3 x)
{
    const uint k = 1103515245u;
    
    x = ((x>>8U)^uint3(x.y, x.z, x.x))*k;//((x>>8U)^x.yzx)*k;
    x = ((x>>8U)^uint3(x.y, x.z, x.x))*k;//((x>>8U)^x.yzx)*k;
    x = ((x>>8U)^uint3(x.y, x.z, x.x))*k;//((x>>8U)^x.yzx)*k;
    
    return x;
}

// Integer Hash - III
// - Inigo Quilez, Integer Hash - III, 2017
//   https://www.shadertoy.com/view/4tXyWN
uint iqint3(uint2 x)
{
    uint2 q = 1103515245U * ( (x>>1U) ^ (uint2(x.y, x.x)) );//1103515245U * ( (x>>1U) ^ (x.yx   ) );
    uint  n = 1103515245U * ( (q.x  ) ^ (q.y>>3U) );
    
    return n;
}

uint iqint3_1(uint2 x)
{
    uint2 q = 1103515245U * ( (x>>1U) ^ (uint2(x.y, x.x)) );//1103515245U * ( (x>>1U) ^ (x.yx   ) );
    uint  n = 1103515245U * ( (q.x  ) ^ (q.y>>3U) );

    n = n ^ (n >> 16u);

    return n;
}

// JKiss32
// - David Jones, Good Practice in (Pseudo) Random Number Generation for 
//   Bioinformatics Applications, 2010
//   http://www0.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
uint jkiss32(uint2 p)
{
    uint x = p.x, y = p.y;
    
	uint z = 345678912u, w = 456789123u, c = 0u;
    int t;
    
    y ^= (y<<5u); y ^= (y>>7u); y ^= (y<<22u);
    t = int(z+w+c); 
    z = w; 
    c = uint(t < 0);
    w = uint(t&2147483647);
    x += 1411392427u;
    
    return x + y + w;
}

uint lcg(uint p)
{
	return p * 1664525u + 1013904223u;
}

#define A0 0x67452301u
#define B0 0xefcdab89u
#define C0 0x98badcfeu
#define D0 0x10325476u

uint F(uint3 v) { return (v.x & v.y) | (~v.x & v.z); }
uint G(uint3 v) { return (v.x & v.z) | (v.y & ~v.z); }
uint H(uint3 v) { return v.x ^ v.y ^ v.z; }
uint I(uint3 v) { return v.y ^ (v.x | ~v.z); }

void rotate(uint4* r)
{
    uint temp = r->x;
    r->x = r->y;
    r->y = r->z;
    r->z = r->w;
    r->w = temp;
}

void FF(uint4* v, uint4* r, uint x, uint ac)
{
    v->x = v->y + rotl(v->x + F(uint3(v->y, v->z, v->w)) + x + ac, r->x);

    rotate(r);
    rotate(v);
}

void GG(uint4* v, uint4* r, uint x, uint ac)
{
    v->x = v->y + rotl(v->x + G(uint3(v->y, v->z, v->w)) + x + ac, r->x);
    rotate(r);
    rotate(v);
}

void HH(uint4* v, uint4* r, uint x, uint ac)
{
    v->x = v->y + rotl(v->x + H(uint3(v->y, v->z, v->w)) + x + ac, r->x);
    rotate(r);
    rotate(v);
}

void II(uint4* v, uint4* r, uint x, uint ac)
{
    v->x = v->y + rotl(v->x + I(uint3(v->y, v->z, v->w)) + x + ac, r->x);
    rotate(r);
    rotate(v);
}

uint K(uint i) {
    return uint(abs(sin(float(i)+1.)) * float(0xffffffffu));
}

uint4 md5(uint4 u) {
    uint4 digest = uint4(A0, B0, C0, D0);
    uint4 r, v = digest;
    uint i = 0u;

    uint M[16];
    M[0] = u.x; M[1] = u.y; M[2] = u.z; M[3] = u.w;
    M[4] = M[5] = M[6] = M[7] = M[8] = M[9] = M[10] = M[11] = M[12] = M[13] = M[14] = M[15] = 0u;

    r = uint4(7, 12, 17, 22);
    FF(&v, &r, M[0], K(i++));
    FF(&v, &r, M[1], K(i++));
    FF(&v, &r, M[2], K(i++));
    FF(&v, &r, M[3], K(i++));
    FF(&v, &r, M[4], K(i++));
    FF(&v, &r, M[5], K(i++));
    FF(&v, &r, M[6], K(i++));
    FF(&v, &r, M[7], K(i++));
    FF(&v, &r, M[8], K(i++));
    FF(&v, &r, M[9], K(i++));
    FF(&v, &r, M[10], K(i++));
    FF(&v, &r, M[11], K(i++));
    FF(&v, &r, M[12], K(i++));
    FF(&v, &r, M[13], K(i++));
    FF(&v, &r, M[14], K(i++));
    FF(&v, &r, M[15], K(i++));

    r = uint4(5, 9, 14, 20);
    GG(&v, &r, M[1], K(i++));
    GG(&v, &r, M[6], K(i++));
    GG(&v, &r, M[11], K(i++));
    GG(&v, &r, M[0], K(i++));
    GG(&v, &r, M[5], K(i++));
    GG(&v, &r, M[10], K(i++));
    GG(&v, &r, M[15], K(i++));
    GG(&v, &r, M[4], K(i++));
    GG(&v, &r, M[9], K(i++));
    GG(&v, &r, M[14], K(i++));
    GG(&v, &r, M[3], K(i++));
    GG(&v, &r, M[8], K(i++));
    GG(&v, &r, M[13], K(i++));
    GG(&v, &r, M[2], K(i++));
    GG(&v, &r, M[7], K(i++));
    GG(&v, &r, M[12], K(i++));

    r = uint4(4, 11, 16, 23);
    HH(&v, &r, M[5], K(i++));
    HH(&v, &r, M[8], K(i++));
    HH(&v, &r, M[11], K(i++));
    HH(&v, &r, M[14], K(i++));
    HH(&v, &r, M[1], K(i++));
    HH(&v, &r, M[4], K(i++));
    HH(&v, &r, M[7], K(i++));
    HH(&v, &r, M[10], K(i++));
    HH(&v, &r, M[13], K(i++));
    HH(&v, &r, M[0], K(i++));
    HH(&v, &r, M[3], K(i++));
    HH(&v, &r, M[6], K(i++));
    HH(&v, &r, M[9], K(i++));
    HH(&v, &r, M[12], K(i++));
    HH(&v, &r, M[15], K(i++));
    HH(&v, &r, M[2], K(i++));

    r = uint4(6, 10, 15, 21);
    II(&v, &r, M[0], K(i++));
    II(&v, &r, M[7], K(i++));
    II(&v, &r, M[14], K(i++));
    II(&v, &r, M[5], K(i++));
    II(&v, &r, M[12], K(i++));
    II(&v, &r, M[3], K(i++));
    II(&v, &r, M[10], K(i++));
    II(&v, &r, M[1], K(i++));
    II(&v, &r, M[8], K(i++));
    II(&v, &r, M[15], K(i++));
    II(&v, &r, M[6], K(i++));
    II(&v, &r, M[13], K(i++));
    II(&v, &r, M[4], K(i++));
    II(&v, &r, M[11], K(i++));
    II(&v, &r, M[2], K(i++));
    II(&v, &r, M[9], K(i++));

    return digest + v;
}

uint4 md5_1(uint u)
{
    return md5(uint4(u, 0u, 0u, 0u));
}

uint4 md5_2(uint2 u)
{
    return md5(uint4(u.x, u.y, 0u, 0u));
}

uint4 md5_3(uint3 u)
{
    return md5(uint4(u.x, u.y, u.z, 0u));
}



// Adapted from MurmurHash3_x86_32 from https://github.com/aappleby/smhasher
uint murmur3(uint seed)
{
    uint h = 0u;
    uint k = seed;
    
    k *= c1;
    k = rotl(k,15u);
    k *= c2;

    h ^= k;
    h = rotl(h,13u);	
    h = h*5u+0xe6546b64u;
   
    h ^= 4u; 
    
    return fmix(h);
}

uint murmur3(uint2 seed)
{
    uint h = 0u;
    uint k = seed.x;
    
    k *= c1;
    k = rotl(k,15u);
    k *= c2;

    h ^= k;
    h = rotl(h,13u);
    h = h*5u+0xe6546b64u;

    k = seed.y;

    k *= c1;
    k = rotl(k,15u);
    k *= c2;

    h ^= k;
    h = rotl(h,13u);
    h = h*5u+0xe6546b64u;

    h ^= 8u;

    return fmix(h);
}


uint murmur3(uint3 seed)
{
    uint h = 0u;
    uint k = seed.x;
    
    k *= c1;
    k = rotl(k,15u);
    k *= c2;

    h ^= k;
    h = rotl(h,13u);
    h = h*5u+0xe6546b64u;

    k = seed.y;

    k *= c1;
    k = rotl(k,15u);
    k *= c2;

    h ^= k;
    h = rotl(h,13u);
    h = h*5u+0xe6546b64u;
    
    k = seed.z;

    k *= c1;
    k = rotl(k,15u);
    k *= c2;

    h ^= k;
    h = rotl(h,13u);
    h = h*5u+0xe6546b64u;

    h ^= 12u;

    return fmix(h);
}

uint murmur3(uint4 seed)
{
    uint h = 0u;
    uint k = seed.x;
    
    k *= c1;
    k = rotl(k,15u);
    k *= c2;

    h ^= k;
    h = rotl(h,13u);
    h = h*5u+0xe6546b64u;

    k = seed.y;

    k *= c1;
    k = rotl(k,15u);
    k *= c2;

    h ^= k;
    h = rotl(h,13u);
    h = h*5u+0xe6546b64u;
    
    k = seed.z;

    k *= c1;
    k = rotl(k,15u);
    k *= c2;

    h ^= k;
    h = rotl(h,13u);
    h = h*5u+0xe6546b64u;
    
    k = seed.w;

    k *= c1;
    k = rotl(k,15u);
    k *= c2;

    h ^= k;
    h = rotl(h,13u);
    h = h*5u+0xe6546b64u;

    h ^= 16u;

    return fmix(h);
}


// UE4 RandPCG3D32
uint3 pcg3d(uint3 v)
{
	v = v * 1664525u + 1013904223u;   

    v.x += v.y*v.z;
    v.y += v.z*v.x;
    v.z += v.x*v.y;
    
    v = v ^ (v>>16u);
    
    v.x += v.y*v.z;
    v.y += v.z*v.x;
    v.z += v.x*v.y;
    
    return v;
}

uint3 pcg3d2(uint3 v)
{
	v = v * 1664525u + 1013904223u;   

    v.x += v.y*v.z;
    v.y += v.z*v.x;
    v.z += v.x*v.y;
    
    v = (v<<16u) ^ (v>>16u);
    
    v.x += v.y*v.z;
    v.y += v.z*v.x;
    v.z += v.x*v.y;
    
    return v;
}

// UE4 RandPCG3d16
uint3 pcg3d16(uint3 v)
{
    v = v * 12829u + 47989u;
    v.x += v.y*v.z;
    v.y += v.z*v.x;
    v.z += v.x*v.y;

    v.x += v.y*v.z;
    v.y += v.z*v.x;
    v.z += v.x*v.y;

    v = v >> 16u;

    return v;
}

// UE4 PseudoRandom function
float pseudo(float2 v) {
    v = frac(v/128.)*128.f + float2(-64.340622f, -72.465622f);
    return frac(dot(float3(v.x, v.y, v.x) * float3(v.x, v.y, v.y), float3(20.390625f, 60.703125f, 2.4281209f)));//frac(dot(v.xyx * v.xyy, float3(20.390625f, 60.703125f, 2.4281209f)));
}

uint ranlim32(uint j)
{
    uint u, v, w1, w2, x, y;
    
    v = 2244614371U;
    w1 = 521288629U;
    w2 = 362436069U;

    u = j ^ v;

    u = u * 2891336453U + 1640531513U;
    v ^= v >> 13u; v ^= v << 17u; v ^= v >> 5u;
    w1 = 33378u * (w1 & 0xffffu) + (w1 >> 16u);
    w2 = 57225u * (w2 & 0xffffu) + (w2 >> 16u);

    v = u;

    u = u * 2891336453U + 1640531513U;
    v ^= v >> 13u; v ^= v << 17u; v ^= v >> 5u;
    w1 = 33378u * (w1 & 0xffffu) + (w1 >> 16u);
    w2 = 57225u * (w2 & 0xffffu) + (w2 >> 16u);

    x = u ^ (u << 9); x ^= x >> 17; x ^= x << 6;
    y = w1 ^ (w1 << 17); y ^= y >> 15; y ^= y << 5;

    return (x + v) ^ (y + w2);
}

// SuperFastHash, adapated from http://www.azillionmonkeys.com/qed/hash.html
uint superfast(uint data)
{
	uint hash = 4u, tmp;
    
    hash += data & 0xffffu;
    tmp = (((data >> 16) & 0xffffu) << 11) ^ hash;
    hash = (hash << 16) ^ tmp;
    hash += hash >> 11;

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    
    return hash;
}

uint superfast(uint2 data)
{
    uint hash = 8u, tmp;

    hash += data.x & 0xffffu;
    tmp = (((data.x >> 16) & 0xffffu) << 11) ^ hash;
    hash = (hash << 16) ^ tmp;
    hash += hash >> 11;
 
    hash += data.y & 0xffffu;
    tmp = (((data.y >> 16) & 0xffffu) << 11) ^ hash;
    hash = (hash << 16) ^ tmp;
    hash += hash >> 11;
 
    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    
    return hash;
}

uint superfast(uint3 data)
{
    uint hash = 8u, tmp;

    hash += data.x & 0xffffu;
    tmp = (((data.x >> 16) & 0xffffu) << 11) ^ hash;
    hash = (hash << 16) ^ tmp;
    hash += hash >> 11;
 
    hash += data.y & 0xffffu;
    tmp = (((data.y >> 16) & 0xffffu) << 11) ^ hash;
    hash = (hash << 16) ^ tmp;
    hash += hash >> 11;
    
    hash += data.z & 0xffffu;
    tmp = (((data.z >> 16) & 0xffffu) << 11) ^ hash;
    hash = (hash << 16) ^ tmp;
    hash += hash >> 11;
 
    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    
    return hash;
}

uint superfast(uint4 data)
{
    uint hash = 8u, tmp;

    hash += data.x & 0xffffu;
    tmp = (((data.x >> 16) & 0xffffu) << 11) ^ hash;
    hash = (hash << 16) ^ tmp;
    hash += hash >> 11;
 
    hash += data.y & 0xffffu;
    tmp = (((data.y >> 16) & 0xffffu) << 11) ^ hash;
    hash = (hash << 16) ^ tmp;
    hash += hash >> 11;
    
    hash += data.z & 0xffffu;
    tmp = (((data.z >> 16) & 0xffffu) << 11) ^ hash;
    hash = (hash << 16) ^ tmp;
    hash += hash >> 11;
    
    hash += data.w & 0xffffu;
    tmp = (((data.w >> 16) & 0xffffu) << 11) ^ hash;
    hash = (hash << 16) ^ tmp;
    hash += hash >> 11;
 
    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    
    return hash;
}

uint2 tea(int t, uint2 p)
{
    uint s = 0u;

    for( int i = 0; i < t; i++) {
        s += 0x9E3779B9u;
        p.x += (p.y<<4u)^(p.y+s)^(p.y>>5u);
        p.y += (p.x<<4u)^(p.x+s)^(p.x>>5u);
    }
    return uint2(p.x, p.y);//p.xy;
}

float trig(float2 p)
{
    return frac(43757.5453*sin(dot(p, float2(12.9898,78.233))));
}

// Wang hash, described on http://burtleburtle.net/bob/hash/integer.html
// original page by Thomas Wang 404
uint wang(uint v)
{
    v = (v ^ 61u) ^ (v >> 16u);
    v *= 9u;
    v ^= v >> 4u;
    v *= 0x27d4eb2du;
    v ^= v >> 15u;
    return v;
}

// 128-bit xorshift
//  - Marsaglia, Xorshift RNGs, Journal of Statistical Software, v8n14, 2003
uint4 xorshift128(uint4 v)
{
    v.w ^= v.w << 11u;
    v.w ^= v.w >> 8u;
    v = uint4(v.w, v.x, v.y, v.z); //v.wxyz;
    v.x ^= v.y;
    v.x ^= v.y >> 19u;
    return v;
}

// 32-bit xorshift
//  - Marsaglia, Xorshift RNGs, Journal of Statistical Software, v8n14, 2003
uint xorshift32(uint v) {
    v ^= v << 13u;
    v ^= v >> 17u;
    v ^= v << 5u;
    return v;
}

// xxhash (https://github.com/Cyan4973/xxHash)
//   From https://www.shadertoy.com/view/Xt3cDn
uint xxhash32(uint p)
{
    const uint PRIME32_2 = 2246822519U, PRIME32_3 = 3266489917U;
    const uint PRIME32_4 = 668265263U, PRIME32_5 = 374761393U;
    /*uint h32 = p + PRIME32_5;
    h32 = PRIME32_4*((h32 << 17) | (h32 >> (32 - 17)));
    h32 = PRIME32_2*(h32^(h32 >> 15));
    h32 = PRIME32_3*(h32^(h32 >> 13));
    return h32^(h32 >> 16);
    */
    
    uint h32 = p + PRIME32_5;
    

    h32 += p * PRIME32_3;
    h32 = rotl(h32, 17) * PRIME32_4;

    h32 ^= h32 >> 15;
    h32 *= PRIME32_2;
    h32 ^= h32 >> 13;
    h32 *= PRIME32_3;
    h32 ^= h32 >> 16;
    return h32;
}

uint xxhash32(uint2 p)
{
    const uint PRIME32_2 = 2246822519U, PRIME32_3 = 3266489917U;
	const uint PRIME32_4 = 668265263U, PRIME32_5 = 374761393U;
    uint h32 = p.y + PRIME32_5 + p.x*PRIME32_3;
    h32 = PRIME32_4*((h32 << 17) | (h32 >> (32 - 17)));
    h32 = PRIME32_2*(h32^(h32 >> 15));
    h32 = PRIME32_3*(h32^(h32 >> 13));
    return h32^(h32 >> 16);
}

uint xxhash32(uint3 p)
{
    const uint PRIME32_2 = 2246822519U, PRIME32_3 = 3266489917U;
    const uint PRIME32_4 = 668265263U, PRIME32_5 = 374761393U;
    uint h32 =  p.z + PRIME32_5 + p.x*PRIME32_3;
    h32 = PRIME32_4*((h32 << 17) | (h32 >> (32 - 17)));
    h32 += p.y * PRIME32_3;
    h32 = PRIME32_4*((h32 << 17) | (h32 >> (32 - 17)));
    h32 = PRIME32_2*(h32^(h32 >> 15));
    h32 = PRIME32_3*(h32^(h32 >> 13));
    return h32^(h32 >> 16);
}

uint xxhash32(uint4 p)
{
    const uint PRIME32_2 = 2246822519U, PRIME32_3 = 3266489917U;
    const uint PRIME32_4 = 668265263U, PRIME32_5 = 374761393U;
    uint h32 =  p.w + PRIME32_5 + p.x*PRIME32_3;
    h32 = PRIME32_4*((h32 << 17) | (h32 >> (32 - 17)));
    h32 += p.y * PRIME32_3;
    h32 = PRIME32_4*((h32 << 17) | (h32 >> (32 - 17)));
    h32 += p.z * PRIME32_3;
    h32 = PRIME32_4*((h32 << 17) | (h32 >> (32 - 17)));
    h32 = PRIME32_2*(h32^(h32 >> 15));
    h32 = PRIME32_3*(h32^(h32 >> 13));
    return h32^(h32 >> 16);
}


// Hash without Sine, https://www.shadertoy.com/view/4djSRW
float hashwithoutsine11(float p)
{
    p = frac(p * .1031);
    p *= p + 19.19;
    p *= p + p;
    return frac(p);
}

float hashwithoutsine12(float2 p)
{
    float3 p3  = frac(float3(p.x, p.y, p.x) * .1031);
    p3 += dot(p3, float3(p3.y, p3.z, p3.x) + 19.19);
    return frac((p3.x + p3.y) * p3.z);
}

float hashwithoutsine13(float3 p3)
{
    p3  = frac(p3 * .1031);
    p3 += dot(p3, float3(p3.y, p3.z, p3.x) + 19.19);
    return frac((p3.x + p3.y) * p3.z);
}

float2 hashwithoutsine21(float p)
{
    float3 p3 = frac(float3(p) * float3(.1031, .1030, .0973));
    p3 += dot(p3, float3(p3.y, p3.z, p3.x) + 19.19);
    return frac((float2(p3.x, p3.x) + float2(p3.y, p3.z)) * float2(p3.z, p3.y));

}

float2 hashwithoutsine22(float2 p)
{
    float3 p3 = frac(float3(p.x, p.y, p.x) * float3(.1031, .1030, .0973));
    p3 += dot(p3, float3(p3.y, p3.z, p3.x) + 19.19);
    return frac((float2(p3.x, p3.x) + float2(p3.y, p3.z)) * float2(p3.z, p3.y));

}

float2 hashwithoutsine23(float3 p3)
{
    p3 = frac(p3 * float3(.1031, .1030, .0973));
    p3 += dot(p3, float3(p3.y, p3.z, p3.x) + 19.19);
    return frac((float2(p3.x, p3.x) + float2(p3.y, p3.z)) * float2(p3.z, p3.y));
}

float3 hashwithoutsine31(float p)
{
   float3 p3 = frac(float3(p) * float3(.1031, .1030, .0973));
   p3 += dot(p3, float3(p3.y, p3.z, p3.x) + 19.19);
   return frac((float3(p3.x, p3.x, p3.y) + float3(p3.y, p3.z, p3.z)) * float3(p3.z, p3.y, p3.x)); 
}

float3 hashwithoutsine32(float2 p)
{
	float3 p3 = frac(float3(p.x, p.y, p.x) * float3(.1031, .1030, .0973));//frac(float3(p.xyx) * float3(.1031, .1030, .0973));
    p3 = p3 + dot(p3, float3(p3.y, p3.x, p3.z) + 19.19);//dot(p3, p3.yxz+19.19);
    return frac((float3(p3.x, p3.x, p3.y) + float3(p3.y, p3.z, p3.z)) * float3(p3.z, p3.y, p3.x));//frac((p3.xxy+p3.yzz)*p3.zyx);
}

float3 hashwithoutsine33(float3 p3)
{
    p3 = frac(p3 * float3(.1031, .1030, .0973));
    p3 += dot(p3, float3(p3.y, p3.x, p3.z) + 19.19);
    return frac((float3(p3.x, p3.x, p3.y) + float3(p3.y, p3.x, p3.x)) * float3(p3.z, p3.y, p3.x));
}

float4 hashwithoutsine41(float p)
{
    float4 p4 = frac(float4(p) * float4(.1031, .1030, .0973, .1099));
    p4 += dot(p4, float4(p4.w, p4.z, p4.x, p4.y) + 19.19);
    return frac((float4(p4.x, p4.x, p4.y, p4.z) + float4(p4.y, p4.z, p4.z, p4.w)) * float4(p4.z, p4.y, p4.w, p4.x));
}

float4 hashwithoutsine42(float2 p)
{
    float4 p4 = frac(float4(p.x, p.y, p.x, p.y) * float4(.1031, .1030, .0973, .1099));
    p4 += dot(p4, float4(p4.w, p4.z, p4.x, p4.y) + 19.19);
    return frac((float4(p4.x,p4.x,p4.y, p4.z ) + float4(p4.y, p4.z, p4.z, p4.w)) * float4(p4.z, p4.y, p4.w, p4.x));
}

float4 hashwithoutsine43(float3 p)
{
    float4 p4 = frac(float4(p.x, p.y, p.z, p.x)  * float4(.1031, .1030, .0973, .1099));
    p4 += dot(p4, float4(p4.w, p4.z, p4.x, p4.y) + 19.19);
    return frac((float4(p4.x, p4.x, p4.y, p4.z) + float4(p4.y, p4.z, p4.z, p4.w)) * float4(p4.z, p4.y, p4.w, p4.x));
}

float4 hashwithoutsine44(float4 p4)
{
    p4 = frac(p4  * float4(.1031, .1030, .0973, .1099));
    p4 += dot(p4, float4(p4.w, p4.z, p4.x, p4.y) + 19.19);
    return frac((float4(p4.x, p4.x, p4.y, p4.z) + float4(p4.y, p4.z, p4.z, p4.w)) * float4(p4.z, p4.y, p4.w, p4.x));
}

uint taus(uint z, int s1, int s2, int s3, uint m)
{
	uint b = (((z << s1) ^ z) >> s2);
    return (((z & m) << s3) ^ b);
}

// https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch37.html
uint hybridtaus(uint4 z)
{
    for (int i = 0; i < 1; i++) {
        z.x = taus(z.x, 13, 19, 12, 4294967294u);
        z.y = taus(z.y, 2, 25, 4, 4294967288u);
        z.z = taus(z.z, 3, 11, 17, 4294967280u);
        z.w = z.w * 1664525u + 1013904223u;
    }
    
    return z.x ^ z.y ^ z.z ^ z.w;
}

uint3 Rand3DPCG32(uint3 v)
{
    // Linear congruential step.
    v = v * 1664525u + 1013904223u;

    // swapping low and high bits makes all 32 bits pretty good
    v = v * (1u << 16u) + (v >> 16u);

    // final shuffle
    v.x += v.y*v.z;
    v.y += v.z*v.x;
    v.z += v.x*v.y;
    v.x += v.y*v.z;
    v.y += v.z*v.x;
    v.z += v.x*v.y;

    return v;
}

uint4 Rand4DPCG32(uint4 v)
{
    v = v * 1664525u + 1013904223u;

    v = v * (1u << 16u) + (v >> 16u);

    v.x += v.y*v.w;
    v.y += v.z*v.x;
    v.z += v.x*v.y;
    v.w += v.y*v.z;
    v.x += v.y*v.w;
    v.y += v.z*v.x;
    v.z += v.x*v.y;
    v.w += v.y*v.z;

    return v;
}

uint4 pcg4d(uint4 v)
{
    v = v * 1664525u + 1013904223u;

    v.x += v.y*v.w;
    v.y += v.z*v.x;
    v.z += v.x*v.y; 
    v.w += v.y*v.z;

    v = v ^ (v >> 16u);

    v.x += v.y*v.w;
    v.y += v.z*v.x;
    v.z += v.x*v.y;
    v.w += v.y*v.z;

    return v;

    /*
	v = v * 1664525u + 1013904223u;   
    
    v.x += v.y*v.w;
    v.y += v.z*v.x;
    v.z += v.x*v.y;
    v.w += v.y*v.z;


    v.x += v.y*v.w;
    v.y += v.z*v.x;
    v.z += v.x*v.y;
    v.w += v.y*v.z;

    v = v ^ (v>>16u);

    return v;
    
    */
}

uint2 pcg2d(uint2 v)
{
    v = v * 1664525u + 1013904223u;

    v.x += v.y * 1664525u;
    v.y += v.x * 1664525u;

    v = v ^ (v>>16u);

    v.x += v.y * 1664525u;
    v.y += v.x * 1664525u;

    v = v ^ (v>>16u);
    
    return v;
}

uint3 hashmul( uint u )
{
    uint n = esgtsa(u);

    return uint3(n, n * 16807u, n * 48271u);// n * 69621u);
}

uint3 hashadd( uint u )
{
    return uint3(esgtsa(u), esgtsa(u + 16807u), esgtsa(u + 48271u));
}

uint pcg( uint v )
{
/*
    uint initstate = 42u;
    uint intseq = 54u;

    uint state = 0u;
    uint inc = (initseq << 1u) | 1u;
    
    state = state * 6364136223846793005ULL + inc;
    state += initstate;
    state = state * 6364136223846793005ULL + inc;
    
    uint oldstate = state;
    uint xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u);
    uint rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
*/

/*
	v = v * 1664525u + 1013904223u;
    v = (( v >> 10u ) ^ v) >> 11u;
    uint rot = v >> 27u;
    return (v >> rot) | (v << ((-rot) & 31));
*/
/*
    uint state = 0u;
    state = state * 747796405U + 2891336453U;
    state += initstate;
    state = state * 747796405U + 2891336453U;
*/
    uint state = v * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;


}


uint3 test(uint3 v) {
 
    uint4 p = uint4(v.x, v.y, v.z, v.x ^ v.y ^ v.z);

    p = p * 1664525u + 1013904223u;
    
    p = p + uint4(p.w, p.x, p.y, p.z) * uint4(p.z, p.w, p.x, p.y);
    p = p ^ uint4(p.w, p.x, p.y, p.z) >> 16u;
    p = p + uint4(p.w, p.x, p.y, p.z) * uint4(p.z, p.w, p.x, p.y);
    p = p ^ uint4(p.w, p.x, p.y, p.z) >> 16u;
    p = p + uint4(p.w, p.x, p.y, p.z) * uint4(p.z, p.w, p.x, p.y);

    return uint3(p.x, p.y, p.z);
}



// New hash functions

// https://prng.di.unimi.it/
uint xoshiro128plusplus(uint4 s)
{
	uint t = s.y << 9;

	s.z ^= s.x;
	s.w ^= s.y;
	s.y ^= s.z;
	s.x ^= s.w;

	s.z ^= t;

	s.w = rotl(s.w, 11u);

	return rotl(s.x + s.w, 7u) + s.x;
}

// https://prng.di.unimi.it/
uint xoshiro128starstar(uint4 s)
{
	uint t = s.y << 9;

	s.z ^= s.x;
	s.w ^= s.y;
	s.y ^= s.z;
	s.x ^= s.w;

	s.z ^= t;
    
	s.w = rotl(s.w, 11u);
        
   	return rotl(s.y * 5u, 7u) * 9u;
}


// https://nullprogram.com/blog/2018/07/31/
uint lowbias32(uint x)
{
    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;
    x *= 0x846ca68bU;
    x ^= x >> 16;
    return x;
}


// https://github.com/skeeto/hash-prospector/issues/23#issuecomment-1401616701
uint pxq(uint v)
{
    uint state = (v | 1u) ^ (v * v);
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}


// https://github.com/skeeto/hash-prospector/issues/23#issuecomment-1407393298
// https://www.shadertoy.com/view/dllSW7
uint newintegerhash(uint x)
{
    x ^= x >> 15;
    x ^= (x * x) | 1u;
    x ^= x >> 17;
    x *= 0x9E3779B9u;
    x ^= x >> 13;
    return x;
}


// https://github.com/skeeto/hash-prospector/issues/19#issuecomment-1120105785
uint prospector_best(uint x)
{
    x = (x ^ (x >> 16)) * 0x21f0aaadU;
    x = (x ^ (x >> 15)) * 0x735a2d97U;
    return x ^ (x >> 15);
}


// https://www.burtleburtle.net/bob/hash/doobs.html
uint one_at_a_time_1(uint key)
{
    uint hash = key;
    hash += (hash << 10);
    hash ^= (hash >> 6);
        
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

uint one_at_a_time_2(uint2 key)
{
    uint hash = 0u;
    /*    
    for (int i = 0; i < 2; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    */

    hash += key.x;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += key.y;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

uint one_at_a_time_3(uint3 key)
{
    uint hash = 0u;
   
    /* 
    for (int i = 0; i < 3; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    */
    hash += key.x;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += key.y;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += key.z;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

uint one_at_a_time_4(uint4 key)
{
    uint hash = 0u;
    
    /*
    for (int i = 0; i < 4; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    */
    hash += key.x;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += key.y;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += key.z;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += key.w;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

// mix(a,b,c) from https://www.burtleburtle.net/bob/c/lookup3.c
uint3 lookup3(uint3 v)
{
    v.x -= v.z;  v.x ^= rotl(v.z, 4u);  v.z += v.y;
    v.y -= v.x;  v.y ^= rotl(v.x, 6u);  v.x += v.z;
    v.z -= v.y;  v.z ^= rotl(v.y, 8u);  v.y += v.x;
    v.x -= v.z;  v.x ^= rotl(v.z,16u);  v.z += v.y;
    v.y -= v.x;  v.y ^= rotl(v.x,19u);  v.x += v.z;
    v.z -= v.y;  v.z ^= rotl(v.y, 4u);  v.y += v.x;
    return v;
}


// https://graphics.pixar.com/library/MultiJitteredSampling/paper.pdf
//float pixar_randfloat(uint2 v)
uint pixar_randfloat(uint2 v)
{
    v.y *= 0xa399d265u; // 0x711ad6a5u

    v.x ^= v.y;
    v.x ^= v.x >> 17u;
    v.x ^= v.x >> 10u; v.x *= 0xb36534e5u;
    v.x ^= v.x >> 12u;
    v.x ^= v.x >> 21u; v.x *= 0x93fc4795u;
    v.x ^= 0xdf6e307fu;
    v.x ^= v.x >> 17; v.x *= 1u | v.y >> 18u;
    //return float(v.x) * (1.0f / 4294967808.f);
    return v.x;
}
