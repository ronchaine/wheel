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
/*
      template<flags_t T>
      class Triangle : public Renderable
      {
      };

      template<>
      class Triangle<VERTEX_POSITION | VERTEX_TEXTURE> : public Renderable
      {
      };

      class Rectangle : public Renderable
      {
      };
*/
      class Triangle : public interface::Renderable
      {
         private:
         protected: 
         public:
      };
   }
}

#endif