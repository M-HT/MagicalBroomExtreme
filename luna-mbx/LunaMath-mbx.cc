#include "Luna-mbx.h"

#if (defined(_WIN32) || defined(__WIN32__) || (__WINDOWS__))
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif
#include <ctime>
#include <cstdlib>
#include <cmath>


static long RandTbl[RANDTABLE_MAX];
static long SinTbl[SINTABLE_SIZE];
static long AtanTable[ATANTABLE_SIZE];


void LunaMath::Init( void )
{
#if (defined(_WIN32) || defined(__WIN32__) || (__WINDOWS__))
    srand(time(0) * timeGetTime());
#else
    time_t t0 = time(0);
    struct timespec t1;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    srand(t0 * ((t1.tv_sec * 1000) + (t1.tv_nsec / 1000000)));
#endif

    for (int angle = 0; angle < SINTABLE_SIZE; angle++)
    {
        SinTbl[angle] = std::sin(angle * (2 * M_PI / SINTABLE_SIZE)) * 65536;
    }

    for (int angle = 0; angle < ATANTABLE_SIZE; angle++)
    {
        AtanTable[angle] = std::tan(angle * (2 * M_PI / SINTABLE_SIZE)) * ATANTABLE_SIZE;
    }

    for (int num = 0; num < RANDTABLE_MAX; num++)
    {
        RandTbl[num] = abs(rand() * rand());
    }
}

void LunaMath::UnInit( void )
{
    return;
}

long LunaMath::Rand( long Start, long End )
{
    static unsigned int no = 0;
    no++;
    return Start + (RandTbl[no & RANDTABLE_MASK] % ((End - Start) + 1));
}

long LunaMath::FlatSpeed( long start, long end, long all, long now )
{
    return (((end - start) * now) / all) + start;
}

float LunaMath::FlatSpeed( float start, float end, long all, long now )
{
    return (((end - start) * now) / all) + start;
}

long LunaMath::AddSpeed( long start, long end, long all, long now )
{
    return ((((now * (now + 1)) >> 1) * (end - start)) / ((all * (all + 1)) >> 1)) + start;
}

float LunaMath::AddSpeed( float start, float end, long all, long now )
{
    return ((((now * (now + 1)) >> 1) * (end - start)) / ((all * (all + 1)) >> 1)) + start;
}

long LunaMath::SubSpeed( long start, long end, long all, long now )
{
    return ((((now * (now + 1)) >> 1) * (end - start)) / ((all * (all + 1)) >> 1)) + start;
}

float LunaMath::SubSpeed( float start, float end, long all, long now )
{
    return ((((now * (now + 1)) >> 1) * (end - start)) / ((all * (all + 1)) >> 1)) + start;
}

float LunaMath::Bezier( float p1, float p2, float p3, float t )
{
    double t1 = 1.0 - t;
    double b1 = (t1 * t1 * p1);
    double b2 = (t1 * p2 * t);
    return (b1 + (b2 + b2)) + (p3 * t * t);
}

float LunaMath::Neville( float p1, float p2, float p3, float t )
{
    double t2 = t + t;
    p1 = ((p2 - p1) * (t2 - 1.0)) + p2;
    t2 -= 2.0;
    double n1 = ((p3 - p2) * t2) + p3;
    return (((n1 - p1) * t2) * 0.5) + n1;
}

void LunaMath::RotatePoint( long *x, long *y, long angle )
{
    long anglecos = (angle + (SINTABLE_SIZE / 4)) & SINTABLE_MASK;
    angle &= SINTABLE_MASK;
    long xca = (*x * (long long)SinTbl[anglecos]) >> 16;
    long yca = (*y * (long long)SinTbl[anglecos]) >> 16;
    long xsa = (*x * (long long)SinTbl[angle]) >> 16;
    long ysa = (*y * (long long)SinTbl[angle]) >> 16;
    *x = xca - ysa;
    *y = xsa + yca;
}

void LunaMath::RotatePoint( float *x, float *y, long angle )
{
    angle &= SINTABLE_MASK;
    float ca = std::cos((float)(angle * (float)(2 * M_PI / SINTABLE_SIZE)));
    float sa = std::sin((float)(angle * (float)(2 * M_PI / SINTABLE_SIZE)));
    float y1 = (sa * *x) + (ca * *y);
    *x = (ca * *x) - (sa * *y);
    *y = y1;
}

long double LunaMath::GetPai( void )
{
    return M_PIl;
}

long LunaMath::Sin( long angle )
{
    return SinTbl[angle & SINTABLE_MASK];
}

long LunaMath::Cos( long angle )
{
    return SinTbl[(angle + (SINTABLE_SIZE / 4)) & SINTABLE_MASK];
}

long LunaMath::Sin( long angle, long scale )
{
    return (SinTbl[angle & SINTABLE_MASK] * (long long)scale) >> 16;
}

long LunaMath::Cos( long angle, long scale )
{
    return (SinTbl[(angle + (SINTABLE_SIZE / 4)) & SINTABLE_MASK] * (long long)scale) >> 16;
}

long LunaMath::Atan( long x, long y )
{
    if (x == 0)
    {
        return (y >= 0)?ATANPOS_1:ATANPOS_3;
    }

    if (y == 0)
    {
        return (x >= 0)?0:ATANPOS_2;
    }

    if (x >= 0)
    {
        if (y >= 0)
        {
            if (x < y) return ATANPOS_1 - AtanTable[(x << ATANPOS_SHIFT) / y];
            else       return AtanTable[(y << ATANPOS_SHIFT) / x];
        }
        else
        {
            if (x < (-y)) return AtanTable[(x << ATANPOS_SHIFT) / (-y)] + ATANPOS_3;
            else          return ATANPOS_4 - AtanTable[((-y) << ATANPOS_SHIFT) / x];
        }
    }
    else
    {
        if (y >= 0)
        {
            if ((-x) < y) return AtanTable[((-x) << ATANPOS_SHIFT) / y] + ATANPOS_1;
            else          return ATANPOS_2 - AtanTable[(y << ATANPOS_SHIFT) / (-x)];
        }
        else
        {
            if ((-x) < (-y)) return ATANPOS_3 - AtanTable[((-x) << ATANPOS_SHIFT) / (-y)];
            else             return AtanTable[((-y) << ATANPOS_SHIFT) / (-x)] + ATANPOS_2;
        }
    }
}

float __cdecl LunaMath::SinF( float angle )
{
    return std::sin(angle);
}

float __cdecl LunaMath::CosF( float angle )
{
    return std::cos(angle);
}

float __cdecl LunaMath::TanF( float angle )
{
    return std::tan(angle);
}

void __cdecl LunaMath::SinCosF( float angle, float *s, float *c )
{
    *c = std::cos(angle);
    *s = std::sin(angle);
}

