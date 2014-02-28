/*!
   @file
   \brief Common renderable functions
*/
#include "wheel_module_video.h"

namespace wheel
{
   Renderable::Renderable(const string& name) : name(name)
   {
   }

   uint32_t Renderable::AddSpec(vertex_type_t d_type, float* in_data, size_t data_size, size_t elemcount)
   {
      return WHEEL_UNIMPLEMENTED_FEATURE;
   }

   uint32_t Renderable::AddSpec(vertex_type_t d_type, const string& data_src)
   {
      return WHEEL_UNIMPLEMENTED_FEATURE;
   }

}