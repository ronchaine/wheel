/*!
   @file
   \brief Common geometry declarations
*/

#ifndef WHEEL_GEOMETRY_COMMON_HEADER
#define WHEEL_GEOMETRY_COMMON_HEADER

#include "wheel_core_common.h"
#include "wheel_core_debug.h"
#include "wheel_module_video.h"

namespace wheel
{
   namespace geometry
   {
      namespace internal
      {
         class BasicShape : public interface::Renderable
         {
            private:
            protected:
            public:
               BasicShape(const string& name) : interface::Renderable(name) {}
         };
      }
   }
}

#endif