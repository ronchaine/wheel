/*!
   @file
   \brief Matrix core class
   \author Jari Ronkainen
*/

#ifndef WHEEL_MATH_MATRIX
#define WHEEL_MATH_MATRIX

/*
#include "wheel_math_vector_base.hpp"

#include "wheel_math_vector_2d.hpp"
#include "wheel_math_vector_3d.hpp"

// Simpler names for types
namespace wheel
{
   namespace math
   {
      #ifdef WHEEL_USE_SSE
      #endif

      #ifdef WHEEL_USE_NEON
      #endif

      #if !defined WHEEL_USE_SSE && !defined WHEEL_USE_NEON
         template<typename T>
         using vec2 = vector<T, 2>;

         template<typename T>
         using vec3 = vector<T, 3>;

         template<typename T>
         using vec4 = vector<T, 4>;

         typedef vec2<float> vec2f;
         typedef vec3<float> vec3f;
         typedef vec4<float> vec4f;
      #endif
   }
}

*/

#ifndef WHEEL_NAMESPACE_MATH_ALIASES_DEFINED
#define WHEEL_NAMESPACE_MATH_ALIASES_DEFINED
namespace wmath = wheel::math;
#endif //WHEEL_NAMESPACE_ALIASES_DEFINED

#endif