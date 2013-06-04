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

namespace wheel
{
   class Buffer
   {
      private:
         size_t   size;
         size_t   pos;

         uint8_t* data;

      public:

   };

   class ResourceLibrary
   {
      private:
         std::unordered_map<string, Buffer*> cache;

      public:
         uint32_t Add(const string& filename);
         uint32_t Remove(const string& filename);
   };

   extern ResourceLibrary library;
}


/*
   uint32_t load_file(const string& name)
   {
      if (!PHYSFS_exists(name.std_str().c_str()))
         return WHEEL_RESOURCE_UNAVAILABLE;

      uint8_t* data = nullptr;

      PHYSFS_file* in = PHYSFS_openRead(name.std_str().c_str());
      size_t len = PHYSFS_fileLength(in);

      data = (uint8_t*) malloc(len + 1);

      PHYSFS_read(in, data, 1, len);
      PHYSFS_close(in);

      used_memory += len;
      data = (uint8_t*) malloc(len + 1);

      PHYSFS_read(in, data, 1, len);
      PHYSFS_close(in);

      buffer_t newbuf;

      newbuf.size = len;
      newbuf.data = data;
      newbuf.pos = 0;

      std::pair<std::string, buffer_t> pair;
      pair.first = filename;
      pair.second = newbuf;

      assert(newbuf.data == data);

      cachedfiles.insert(pair);

      return &cachedfiles[filename];
*/     

#endif