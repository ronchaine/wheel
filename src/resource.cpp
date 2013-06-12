/*!
   @file
   \brief Resource management
   \author Jari Ronkainen
*/

#include "../include/wheel_core_resource.h"

#include <physfs.h>

namespace wheel
{
   namespace internal
   {
      std::unordered_map<string, buffer_t*> file_cache;
      size_t cache_memory = 0;
   }

   bool IsCached(const string& filename)
   {
      if (internal::file_cache.count(filename) > 0)
         return true;

      return false;
   }

   uint32_t Buffer(const string& filename)
   {
      if (IsCached(filename))
         return WHEEL_OK;

      if (!PHYSFS_exists(filename.std_str().c_str()))
         return WHEEL_RESOURCE_UNAVAILABLE;

      PHYSFS_file* in = PHYSFS_openRead(filename.std_str().c_str());

      if (in == nullptr)
         return WHEEL_RESOURCE_UNAVAILABLE;

      size_t len = PHYSFS_fileLength(in);

      buffer_t* data = new buffer_t;
      data->resize(len+1);

      PHYSFS_read(in, &data->at(0), 1, len);
      PHYSFS_close(in);

      std::pair<string, buffer_t*> pair;

      pair.first = filename;
      pair.second = data;

      internal::file_cache.insert(pair);

      internal::cache_memory += pair.first.length() * sizeof(char32_t) + pair.second->size();

      return WHEEL_OK;
   }

   void EmptyCache()
   {
      for (auto it : internal::file_cache)
         delete it.second;

      internal::cache_memory = 0;
   }

   void DeleteBuffer(const string& filename)
   {
      if (!IsCached(filename))
         return;

      internal::cache_memory -= filename.length() * sizeof(char32_t) + internal::file_cache[filename]->size();

      delete internal::file_cache[filename];
      internal::file_cache.erase(filename);

      return;
   }

   buffer_t* GetBuffer(const string& filename)
   {
      if (!IsCached(filename))
         if (Buffer(filename) != WHEEL_OK)
            return nullptr;

      return internal::file_cache[filename];
   }

   uint32_t AddToPath(const string& path, const string& mountpoint)
   {
      if (PHYSFS_mount(path.std_str().c_str(), mountpoint.std_str().c_str(), 1) == 0)
         return WHEEL_RESOURCE_UNAVAILABLE;

      return WHEEL_OK;
   }

   size_t BufferSize(const string& filename)
   {
      if (!IsCached(filename))
         return 0;

      return internal::file_cache[filename]->size();
   }
}