/*!
   @file
   \brief 2-dimensional vector specialisations
   \author Jari Ronkainen
*/

#ifndef WHEEL_MATH_VECTOR2D
#define WHEEL_MATH_VECTOR2D

namespace wheel
{
   namespace math
   {
      /*!
      */
      template<typename T>
      class vector<T, 2> : public vector_base<T, 2>
      {
         public:
            //! Default constructor, initialises to zero.
            vector()
            { this->zero(); }
      };
   }
}
#endif