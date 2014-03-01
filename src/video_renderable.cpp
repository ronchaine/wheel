/*!
   @file
   \brief Common renderable functions
*/
#include "wheel_module_video.h"
#include <cstring>

namespace wheel
{
   Renderable::Renderable(const string& name) : name(name), needs_update(true)
   {
   }

   uint32_t Renderable::AddSpec(vertex_type_t d_type, float* in_data, size_t data_size, size_t elemcount)
   {
      needs_update = true;

      // TODO: what to do when resending coordinates?
      if (vertexdata.count(d_type) != 0)
         return WHEEL_UNIMPLEMENTED_FEATURE;

      vertexdata[d_type].datatype = d_type;
      vertexdata[d_type].datafile = "";
      vertexdata[d_type].fdata.resize(data_size / sizeof(float));
      vertexdata[d_type].elem_count = elemcount;
      memcpy(&vertexdata[d_type].fdata[0], in_data, data_size);

      return WHEEL_OK;
   }

   uint32_t Renderable::AddSpec(vertex_type_t d_type, const string& data_src)
   {
      needs_update = true;
      return WHEEL_UNIMPLEMENTED_FEATURE;
   }

}