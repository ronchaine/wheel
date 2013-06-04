/*!
   @file
   \brief Vector core class
   \author Jari Ronkainen
*/

#ifndef WHEEL_MATH_GEOMETRY
#define WHEEL_MATH_GEOMETRY

#include "wheel_math_vector.hpp"

namespace wheel
{
   namespace shapes
   {
      struct vertex_t
      {
         math::vec3f  position;
         math::vec3f  normal;
         math::vec4f  colour;

         vertex_t() {}
         vertex_t(math::vec3f& r, math::vec3f& n, math::vec4f& c)
         {
            position = r;
            normal = n;
            colour = c;
         }
      };

      struct triangle_t
      {
         vertex_t point[3];
      };
   }
}

#ifndef WHEEL_NAMESPACE_MATH_ALIASES_DEFINED
#define WHEEL_NAMESPACE_MATH_ALIASES_DEFINED
namespace wshape = wheel::shapes;
#endif //WHEEL_NAMESPACE_ALIASES_DEFINED

#endif
