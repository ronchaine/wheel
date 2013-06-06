/*!
   @file
   \brief Contains vector base class template
   \author Jari Ronkainen
*/

#ifndef WHEEL_VECTOR_BASE
#define WHEEL_VECTOR_BASE

#include <cstdlib>
#include <cstring>

#include "wheel_math.hpp"

#include <iostream>

namespace wheel
{
   template<typename T>
   inline void unpack_elements(T* ptr)
   {
      return;
   }

   template<typename T, typename ...Targs>
   inline void unpack_elements(T* ptr, T value, Targs&&...t)
   {
      *ptr = value;
      ptr++;

      unpack_elements(ptr, t...);

      return;
   }

   namespace math
   {
      /*!
         Virtual base class that contains common vector operations
      */
      template<typename T, size_t S>
      class vector_base
      {
         public:
            T* element;

            //! Zeroes out elements in the array
            virtual void zero()
            {
               for (size_t i = 0; i < S; ++i)
                  element[i] = 0;
            }

            //! Vector normalisation
            virtual vector_base<T, S> normalise()
            {
               vector_base<T, S> tmp(*this);
               return tmp;
            }

            //! Base constructor.  Allocates memory
            vector_base()
            {
               element = (T*) malloc(S * sizeof(T));
            }

            //! Move constructor.
            vector_base(vector_base<T, S>&& other)
            {
               element = other.element;
               other.element = nullptr;
            }

            //! Copy constructor.
            vector_base(const vector_base<T, S>& other) : vector_base()
            {
               memcpy(&element[0], &other.element[0], S * sizeof(T));
            }

            //! Virtual destructor, releases allocated memory.
            virtual ~vector_base()
            {
               if (element != nullptr)
               {
                  free(element);
                  element = nullptr;
               }
            }

            //! Element access operator
            T& operator[](size_t i)
            {
               return element[i];
            }

            inline void print() const
            {
               std::cout << "(";
               for (size_t i = 0; i < S-1; ++i)
                  std::cout << element[i] << ",";
               std::cout << element[S-1] << ")"  << "\n";
            }


            //! Vector addition
            /*!
               Generic vector addition
            */
            virtual vector_base<T, S> operator+(const vector_base<T, S>& other)
            {
               vector_base<T, S> tmp;

               for (size_t i = 0; i < S; ++i)
                  tmp[i] = (*this)[i] + other.element[i];

               return tmp;
            }

            //! Vector substraction
            /*!
               Generic vector substraction
            */
            virtual vector_base<T, S> operator-(const vector_base<T,S>& other)
            {
               vector_base<T, S> tmp;

               for (size_t i = 0; i < S; ++i)
                  tmp[i] = (*this)[i] - other.element[i];

               return tmp;
            }

            //! Unary minus
            /*!
            */
            virtual vector_base<T, S> operator-()
            {
               vector_base<T, S> tmp;

               for (size_t i = 0; i < S; ++i)
                  tmp[i] = -(*this)[i];

               return tmp;
            }

            //! Inner product
            /*!
            */
            virtual T operator*(const vector_base<T,S>& other)
            {
               T result = 0;

               for (size_t i = 0; i < S; ++i)
                  result += (*this)[i] * other.element[i];

               return result;
            }

            //! Copy assignment
            virtual vector_base<T, S>& operator=(const vector_base<T, S>& other)
            {
               if (this == &other)
                  return *this;

               memcpy(&(this->element[0]), &other.element[0], sizeof(other.element));

               return *this;
            }

            //! Move assignment
            virtual vector_base<T, S>& operator=(vector_base<T, S>&& other)
            {
               if (this != &other)
               {
                  if (this->element != nullptr)
                     free(this->element);

                  this->element = other.element;
                  other.element = nullptr;
               }

               return *this;
            }

            //! Comparison
            virtual bool operator==(const vector_base<T, S>& other)
            {
               if (this == &other)
                  return true;

               for (size_t i = 0; i < S; ++i)
                  if (this->element[i] != other.element[i])
                     return false;

               return true;
            }

            virtual bool operator!=(const vector_base<T, S>& other)
            {
               return !(*this == other);
            }

      };

      /*!
         Generic vector class
      */
      template<typename T, size_t S>
      class vector : public vector_base<T, S>
      {
         public:

            //! Default constructor, initialises to zero.
            vector()
            { this->zero(); }

            //! Conversion from base class
            vector(const vector_base<T, S>& other)
            {
               memcpy(&(this->element[0]), &other.element[0], sizeof(other.element));
            }

            vector(const vector<T, S>& other)
            {
               *this = other;
            }

            template<typename ...T2>
            vector(T2&& ... t)
            {
               unpack_elements(this->element, std::forward<T2>(t)... );
            }

      };



      //===============================================================================

      template<typename T, size_t S>
      void normalise(vector<T, S>& v)
      {
         T value = 0;

         for (size_t i = 0; i < S; ++i)
            value += v.element[i] * v.element[i];

         T r = w_rsqrt(value);

         for (size_t i = 0; i < S; ++i)
            v.element[i] = v.element[i] * r;
      }


      template<typename T>
      vector<T, 3> cross(const vector<T, 3>& v1, const vector<T, 3>& v2)
      {
         vector<T, 3> tmp;

         tmp[0] =  v1.element[1] * v2.element[2] - v2.element[1] * v1.element[2];
         tmp[1] = -v1.element[2] * v2.element[0] + v2.element[2] * v1.element[0];
         tmp[2] =  v1.element[0] * v2.element[1] - v2.element[0] * v1.element[1];

         return tmp;
      }

      //===============================================================================
   }
}

#ifndef WHEEL_NAMESPACE_MATH_ALIASES_DEFINED
#define WHEEL_NAMESPACE_MATH_ALIASES_DEFINED
namespace wmath = wheel::math;
#endif //WHEEL_NAMESPACE_ALIASES_DEFINED

#endif