/*!
   @file
   \brief Basic math functions, NOT optimised for SSE/NEON -yet.
   \author Jari Ronkainen
*/

#ifndef WHEEL_MATH_CORE
#define WHEEL_MATH_CORE

#include <cstdint>
#include <cmath>

namespace wheel
{
   namespace math
   {
      //! Calculates inverse square root, 32-bit floating point version
      /*!
         Uses the familiar "magic rsqrt" algorithm to calculate the
         inverse square root of input value.

         \param value   x in 1/sqrt(x)

         \return value of 1/sqrt(x) in float precision

         \sa http://en.wikipedia.org/wiki/Fast_inverse_square_root
      */
      inline float rsqrt(float value)
      {
         uint32_t i;
         float x2, y;

         x2 = value * 0.5f;
         y = value;

         i = *(uint32_t*) &y;
         i = 0x5f3759df - ( i >> 1 );

         y = *(float*) &i;
         y = y * (1.5f - (x2 * y * y));
         y = y * (1.5f - (x2 * y * y));

         return y;
      }

      //! Calculates inverse square root, 64-bit floating point version
      /*!
         Uses the familiar "magic rsqrt" algorithm to calculate the
         inverse square root of input value.

         \param value   x in 1/sqrt(x)

         \return value of 1/sqrt(x) in double precision

         \sa http://en.wikipedia.org/wiki/Fast_inverse_square_root
      */
      inline double rsqrt(double value)
      {
         uint64_t i;
         double x2, y;

         x2 = value * 0.5f;
         y = value;

         i = *(uint64_t*) &y;
         i = 0x5fe6eb50c7b537a9 - ( i >> 1 );

         y = *(double*) &i;
         y = y * (1.5f - (x2 * y * y));
         y = y * (1.5f - (x2 * y * y));
         y = y * (1.5f - (x2 * y * y));

         return y;
      }

      //! Calculates approximate sine function
      /*!
         This sine function is an approximation of actual sine, it is not as accurate
         as C library sin -function, but it should be accurate enough for most needs.

         \param value   x in sin(x)

         \return value of sin(x) using same precision as the input value

         \sa sin2
         \sa cos
      */
      template<typename T>
      inline T sin(const T& value)
      {
         static const T four_div_pi = 4.0 / 3.14159265358979323846264338327950288;
         static const T minus_four_div_pi_squared = -4.0 / 9.86960440108935861883449099987615114;

         T rval = four_div_pi * value + minus_four_div_pi_squared * value * fabs(value);

         rval = 0.225 * (rval * fabs(rval) - rval) + rval;

         return rval;
      }

      //! Calculates approximate Sine function
      /*!
         The fastest and most inaccurate version of sine function.

         \param value   x in sin(x)

         \return value of sin(x) using same precision as the input value

         \sa sin2
         \sa cos
      */
      template<typename T>
      inline T sin2(const T& value)
      {
         static const T four_div_pi = 4.0 / 3.14159265358979323846264338327950288;
         static const T minus_four_div_pi_squared = -4.0 / 9.86960440108935861883449099987615114;

         T rval = four_div_pi * value + minus_four_div_pi_squared * value * fabs(value);

         return rval;
      }

/*
      template<typename T>
      inline T cosine2(T value)
      {
         static const T four_div_pi = 4.0 / 3.14159265358979323846264338327950288;
         static const T minus_four_div_pi_squared = -4.0 / 9.86960440108935861883449099987615114;
         static const T half_pi = 1.57079632679489661923132169163975144;
         static const T pi = 3.14159265358979323846264338327950288;
         static const T two_pi = 6.28318530717958647692528676655900577;

         value += half_pi;

         T midval = value > pi;

         value -= midval * two_pi;

         T rval = four_div_pi * value + minus_four_div_pi_squared * value * fabs(value);

         rval = 0.225 * (rval * fabs(rval) - rval) + rval;

         return rval;
      }
*/
      //! Calculates approximate cosine function
      /*!
         This cosine function is an approximation of actual cosine, it is not as accurate
         as C library cos -function, but it should be accurate enough for most needs.

         \param value   x in cos(x)

         \return value of cos(x) using same precision as the input value

         \sa sin
         \sa sin2
      */
      template<typename T>
      inline T cos(T value)
      {
         static const T four_div_pi = 4.0 / 3.14159265358979323846264338327950288;
         static const T minus_four_div_pi_squared = -4.0 / 9.86960440108935861883449099987615114;
         static const T half_pi = 1.57079632679489661923132169163975144;
         static const T pi = 3.14159265358979323846264338327950288;
         static const T two_pi = 6.28318530717958647692528676655900577;

         value += half_pi;

         T midval = value > pi;

         value -= midval * two_pi;

         T rval = four_div_pi * value + minus_four_div_pi_squared * value * fabs(value);

         return rval;
      }
   }
}

#ifndef WHEEL_NAMESPACE_MATH_ALIASES_DEFINED
#define WHEEL_NAMESPACE_MATH_ALIASES_DEFINED
namespace wmath = wheel::math;
#endif //WHEEL_NAMESPACE_ALIASES_DEFINED

#endif
