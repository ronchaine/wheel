/*!
   @file
   \brief Vector core class
   \author Jari Ronkainen
*/

#ifndef WHEEL_MATH_ALL
#define WHEEL_MATH_ALL

#if !defined WHEEL_USE_SSE && !defined WHEEL_USE_NEON
   #include "wheel_math_core.hpp"
#endif

#ifdef WHEEL_USE_SSE
   #include "wheel_math_core_sse.hpp"
#elif defined WHEEL_USE_NEON
   #include "wheel_math_core_neon.hpp"
#endif

#include "wheel_math_vector.hpp"
#include "wheel_math_geometry.hpp"

#ifndef WHEEL_NAMESPACE_MATH_ALIASES_DEFINED
#define WHEEL_NAMESPACE_MATH_ALIASES_DEFINED
namespace wmath = wheel::math;
#endif //WHEEL_NAMESPACE_ALIASES_DEFINED

#endif