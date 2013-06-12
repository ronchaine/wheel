/*!
   @file
   \brief Contains resource loading definitions, needs PHYSFS
   \author Jari Ronkainen
*/

#ifndef WHEEL_RESOURCE_HEADER
#define WHEEL_RESOURCE_HEADER

#include "wheel_core_common.h"
#include "wheel_core_string.h"

#include <unordered_map>
#include <vector>

namespace wheel
{
   uint32_t    AddToPath(const string& resource, const string& where);

   buffer_t*   GetBuffer(const string& filename);
   size_t      BufferSize(const string& filename);

   bool        IsCached(const string& filename);

   uint32_t    Buffer(const string& filename);
   void        DeleteBuffer(const string& filename);

   void        EmptyCache();
}

#endif