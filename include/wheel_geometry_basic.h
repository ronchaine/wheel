/*!
   @file
   \brief Basic geometric shapes
*/

#ifndef WHEEL_GEOMETRY_BASIC_HEADER
#define WHEEL_GEOMETRY_BASIC_HEADER

#include "wheel_geometry_common.h"
#include "wheel_module_video.h"

namespace wheel
{
   namespace geometry
   {
      class Triangle : public internal::BasicShape
      {
         private:
         protected: 
         public:
      };

      class Rectangle : public internal::BasicShape
      {
         private:
         protected:
         public:
            Rectangle(string& name, float*);
            Rectangle(string& name);
      };

   }
}

#endif