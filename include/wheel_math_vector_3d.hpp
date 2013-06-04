/*!
   @file
   \brief 3-dimensional vector specialisations
   \author Jari Ronkainen
*/

#ifndef WHEEL_MATH_VECTOR3D
#define WHEEL_MATH_VECTOR3D

namespace wheel
{

   namespace math
   {
      template<typename T>
      class vector<T, 3> : public vector_base<T, 3>
      {
         public:
            //! Default constructor, initialises to zero.
            vector()
            { this->zero(); }

            vector(T x, T y, T z)
            {
               this->element[0] = x;
               this->element[1] = y;
               this->element[2] = z;
            }

            //!
            /*
            */
            vector<T, 3> cross(const vector<T, 3>& other)
            {
               vector<T, 3> tmp;

               tmp[0] =  this->element[1] * other.element[2] - other.element[1] * this->element[2];
               tmp[1] = -this->element[2] * other.element[0] + other.element[2] * this->element[0];
               tmp[2] =  this->element[0] * other.element[1] - other.element[0] * this->element[1];

               return tmp;
            }
      };
   }
}
#endif