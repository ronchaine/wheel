/*!
   @file
   \brief Resource management
   \author Jari Ronkainen
*/

#include "../include/wheel_core_resource.h"

#include <physfs.h>

namespace wheel
{
   /*!
      Default-initialised cache that may be used however seen fit, it was originally made
      so that different modules may have a common cache.
   */
   ResourceLibrary library;

   ResourceLibrary::ResourceLibrary()
   {
      used_mem = 0;
   }
   ResourceLibrary::~ResourceLibrary()
   {
      for (auto it : cache)
         delete it.second;
   }

   /*!
      Checks if a file is already cached

      \param filename      name of a file

      \return  <code>true</code> if the file is in the cache, otherwise <code>false</code>
   */
   bool ResourceLibrary::IsCached(const string& filename)
   {
      if (cache.count(filename) > 0)
         return true;

      return false;
   }

   /*!
      Caches a file.

      \param filename      name of a file to be cached.

      \return  <code>WHEEL_OK</code> on success or an error code depicting the reason for failure.
   */
   uint32_t ResourceLibrary::Cache(const string& filename)
   {
      if (IsCached(filename))
         return WHEEL_OK;

      PHYSFS_file* in = PHYSFS_openRead(filename.std_str().c_str());
      size_t len = PHYSFS_fileLength(in);

      databuffer_t* data = new databuffer_t;
      data->resize(len + 1);

      PHYSFS_read(in, &data->at(0), 1, len);
      PHYSFS_close(in);

      std::pair<string, databuffer_t*> pair;
      pair.first = filename;
      pair.second = data;

      cache.insert(pair);

      used_mem += pair.first.length() * sizeof(char32_t) + pair.second->size();

      return WHEEL_OK;
   }

   /*!
      Removes a file from cache

      \param filename      name of a file to be removed from cache
   */
   void ResourceLibrary::Remove(const string& filename)
   {
      if (!IsCached(filename))
         return;

      used_mem -= filename.length() * sizeof(char32_t) + cache[filename]->size();

      delete cache[filename];

      cache.erase(filename);

      return;
   }

   /*!
      Retrieves pointer to a memory buffer with contents of the file, caches
      the file if it isn't already cached.

      \param filename      name of a file to be read into the buffer

      \return Pointer pointing to a memory buffer or <code>nullptr</code>
   */
   databuffer_t* ResourceLibrary::GetBuffer(const string& filename)
   {
      if (!IsCached(filename))
         if (Cache(filename) != WHEEL_OK)
            return nullptr;

      return cache[filename];
   }

   /*!
      Add an archive or a directory to search path

      \param path    A directory or an archive to be added to the search path.
   */
   uint32_t ResourceLibrary::AddToPath(const string& path)
   {
      if (PHYSFS_addToSearchPath(path.std_str().c_str(), 1) == 0)
         return WHEEL_RESOURCE_UNAVAILABLE;

      return WHEEL_OK;
   }

   /*!
      Add an archive or a directory to search path

      \param path    A directory or an archive to be added to the search path.
   */
   size_t ResourceLibrary::BufferSize(const string& filename)
   {
      if (!IsCached(filename))
         return 0;

      return cache[filename]->size();
   }
}