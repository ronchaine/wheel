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
   typedef std::vector<uint8_t> databuffer_t;

   class ResourceLibrary
   {
      private:
         std::unordered_map<string, databuffer_t*> cache;

         size_t         used_mem;

      public:
         uint32_t       Cache(const string& filename);
         void           Remove(const string& filename);

         databuffer_t*  GetBuffer(const string& filename);
         size_t         BufferSize(const string& filename);

         uint32_t       AddToPath(const string& resource);

         size_t         MemInfo();
         bool           IsCached(const string& filename);

         ResourceLibrary();
        ~ResourceLibrary();
   };

   extern ResourceLibrary library;
}

#endif