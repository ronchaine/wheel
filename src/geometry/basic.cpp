/*!
   @file
   \brief Basic geometric shape functions
*/

#include "../../include/wheel_geometry_basic.h"

namespace wheel
{
   namespace geometry
   {
      Rectangle::Rectangle(const string& name, float*) : BasicShape(name)
      {}

      Rectangle::Rectangle(const string& name) : BasicShape(name)
      {}
   }
}
