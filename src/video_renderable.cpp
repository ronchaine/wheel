/*!
   @file
   \brief Common renderable functions
*/
#include "wheel_module_video.h"
#include <cstring>

namespace wheel
{
   Renderable::Renderable(const string& name) : name(name)
   {
   }

   uint32_t Renderable::AddSpec(vertex_type_t d_type, float* in_data, size_t data_size, size_t elemcount)
   {
      if (vertexdata.count(d_type) != 0)
         return WHEEL_UNIMPLEMENTED_FEATURE;

      vertexdata[d_type].datatype = d_type;
      vertexdata[d_type].datafile = "";
      vertexdata[d_type].fdata.resize(data_size);

      memcpy(&vertexdata[d_type].fdata[0], in_data, data_size);

      return WHEEL_OK;
/*
      if (d_type == VERTEX_TYPE_POSITION)
         for (auto it : vertexdata)
            if (d_type == it.datatype)
            {
               size_t start_elem = it.fdata.size();
               it.fdata.resize(it.fdata.size() + data_size);
               memcpy(&it.fdata[start_elem], in_data, data_size);

               return WHEEL_OK;
            }

      vertex_spec_t vspec;

      vspec.datatype = d_type;
      vspec.datafile = "";
      vspec.fdata.resize(data_size);

      memcpy(&vspec.fdata[0], in_data, data_size);

      vertexdata.push_back(vspec);

      return WHEEL_OK;
*/
   }

   uint32_t Renderable::AddSpec(vertex_type_t d_type, const string& data_src)
   {
      return WHEEL_UNIMPLEMENTED_FEATURE;
   }

}