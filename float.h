#ifndef FLOAT_H
#define FLOAT_H

/** @file */

#include <inttypes.h>
#include <math.h>

#ifdef DOUBLE
typedef double fp;
#else
typedef float fp;
#define atan2 atan2f
#define exp expf
#define sin sinf
#define cos cosf
//#define sqrt sqrtf
#endif

/**
Calculate the sine and cosine of the sum of two angles given the sines and cosines of those angles
@param ca cosine of angle a
@param cb cosine of angle b
@param sa sine of angle a
@param sb sine of angle b
@param c cosine of a+b
@param s sine of a+b

Avoid trig-land! Use trig identities!
\f{eqnarray*}{
\cos(a\pm b)=\cos(a)\cos(b)\mp \sin(a)\sin(b) \\
\sin(a\mp b)=\sin(a)\cos(b)\pm \cos(a)\sin(b)
\f}
*/
static inline void trigp(fp ca, fp cb, fp sa, fp sb, fp &c, fp &s) {
  c=ca*cb+sa*sb;
  s=sa*cb-ca*sb;
}

//Calculate the sine and cosine of the sum of difference between two angles
//given the sines and cosines of those angles
static inline void trigm(fp ca, fp cb, fp sa, fp sb, fp &c, fp &s) {
  c=ca*cb-sa*sb;
  s=sa*cb+ca*sb;
}

constexpr fp PI=3.1415926535897932;

//From Wikipedia:Fast inverse square root. Must be float, not fp. Edited
//to use correct types and avoid strict-aliasing warning on conversion from float to int
static inline float Q_rsqrt( float number ) {
  union floatint {
    float f;
    int32_t i;
  };

  floatint i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = number * 0.5F;
  i.f  = number;        // evil floating point bit level hacking
  i.i  = 0x5f3759df - ( i.i >> 1 );               // what the...?
  y  = i.f;
  y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//  y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

  return y;
}

#endif


