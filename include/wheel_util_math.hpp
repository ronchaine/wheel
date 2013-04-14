/*!
   @file
   \brief Basic vector and matrix classes.
*/

#ifndef WHEEL_UTIL_MATH_HPP
#define WHEEL_UTIL_MATH_HPP

#include <algorithm>

namespace wheel
{
   //! 2-Dimensional vectors
   /*!
   */
   template<typename T>
   class vec2
   {
      public:
         union {
            T _in[2];

            struct {
               T x, y;
            };
            struct {
               T u, v;
            };
         };

         //! Default constructor, initialises the vector values to 0.
         vec2()
         {
            _in[0] = 0.0;
            _in[1] = 0.0;
         }
         //! Default copy constructor
         vec2(const vec2& other)
         {
            // uses assignment overload
            *this = other;
         }

         //! Default move constructor
         vec2(vec2&& other)
         {
            *this = std::move(other);
         }

         //! Initialiser list constructor
         template<typename ... T2>
         vec2(T2&&...t): _in {std::forward<T2>(t)...}
         {}

         //! Access operator
         T& operator[](size_t i)
         {
            return _in[i];
         }

         //! Addition
         T operator+(const T& other) 
         {
            T rval(*this);

            rval.x += other.x;
            rval.y += other.y;

            return rval;
         }

         //! Substraction
         T operator-(const T& other)
         {
            T rval(*this);

            rval.x -= other.x;
            rval.y -= other.y;

            return rval;
         }
   };

}

#endif