/*!
   @file
   \brief Resource management
   \author Jari Ronkainen
*/

#include "../include/wheel_core_resource.h"
#include "../include/wheel_core_debug.h"

#include <physfs.h>

namespace wheel
{
   namespace internal
   {
      std::unordered_map<string, buffer_t*> file_cache;
      size_t cache_memory = 0;
   }

   /*!
      DEBUG__ShowPaths

      \return nothing
   */
   void DEBUG__ShowPaths()
   {
      #ifndef NDEBUG

      log << "=== Search paths ===\n";
      char **i;

      for (i = PHYSFS_getSearchPath(); *i != NULL; i++)
         log << *i << "\n";

      #endif
   }

   /*!
      Creates a directory
   */
   uint32_t CreateDirectory(const char* dir)
   {
      if (PHYSFS_mkdir(dir))
         return WHEEL_OK;
      
      return WHEEL_RESOURCE_UNAVAILABLE;
   }

   /*!
      Set write path

      \return <code>WHEEL_OK</code> on success, otherwise code depicting an error.
   */
   uint32_t SetWritePath(const char* wdir)
   {
      if (PHYSFS_setWriteDir(wdir))
         return WHEEL_OK;

      return WHEEL_RESOURCE_UNAVAILABLE;
   }

   /*!
      \return System user directory.
   */
   const char* UserPath()
   {
      return PHYSFS_getUserDir();
   }

   /*!
      \return Program base directory.
   */
   const char* AppPath()
   {
      return PHYSFS_getBaseDir();
   }

   /*!
      Initialises the filesystem

      \return <code>WHEEL_OK</code>.
   */
   uint32_t Filesystem_Init(int argc, char* argv[])
   {
      if (PHYSFS_init(argv[0]))
      {
         PHYSFS_addToSearchPath(PHYSFS_getBaseDir(), 0);
         
         return WHEEL_OK;
      }

      return WHEEL_ERROR_INIT_FILESYSTEM;
   }
   /*!
      Empties cache and handles cleanup.
   */
   void Filesystem_Deinit()
   {
      EmptyCache();
      PHYSFS_deinit();
   }

   /*!
      Checks if the file is in the cache (has been buffered).

      \return <code>true</code>If file is buffered, otherwise <code>false</code>
   */
   bool IsCached(const string& filename)
   {
      if (internal::file_cache.count(filename) > 0)
         return true;

      return false;
   }

   /*!
      Cache contents of a file.

      \return <code>WHEEL_OK</code> on success, or an error code depicting the error.
   */
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

      PHYSFS_read(in, (void*)data->getptr(), 1, len);
      PHYSFS_close(in);

      std::pair<string, buffer_t*> pair;

      pair.first = filename;
      pair.second = data;

      internal::file_cache.insert(pair);

      internal::cache_memory += pair.first.length() * sizeof(char32_t) + pair.second->size();

      return WHEEL_OK;
   }

   /*!
      Deletes all buffers from the cache and frees the memory.
   */
   void EmptyCache()
   {
      for (auto it : internal::file_cache)
         delete it.second;

      internal::cache_memory = 0;
   }

   /*!
      Deletes a buffer from the cache.
   */
   void DeleteBuffer(const string& filename)
   {
      if (!IsCached(filename))
         return;

      internal::cache_memory -= filename.length() * sizeof(char32_t) + internal::file_cache[filename]->size();

      delete internal::file_cache[filename];
      internal::file_cache.erase(filename);

      return;
   }

   /*!
      Retrieves a pointer to a buffer from the cache.

      \return  pointer to the cached buffer in buffer_t -format.
   */
   const buffer_t* GetBuffer(const string& filename)
   {
      if (!IsCached(filename))
         if (Buffer(filename) != WHEEL_OK)
         {
            return nullptr;
         }

      return internal::file_cache[filename];
   }

   /*!
      Writes a buffer to a file in user directory.
   */
   uint32_t WriteBuffer(const string& file, const buffer_t& buffer)
   {
      PHYSFS_File* handle;

      if((handle = PHYSFS_openWrite(file.std_str().c_str())) == NULL)
      {
         std::cout << "res unavail :: " << PHYSFS_getLastError() << "\n";
         return WHEEL_RESOURCE_UNAVAILABLE;
      }

      // FIXME: After physfs 2.1 goes live, this probably should use writeBytes instead.
      PHYSFS_write(handle, (void*)&buffer[0], buffer.size(), 1);

      if(PHYSFS_close(handle) == 0)
         return WHEEL_RESOURCE_BUSY;

      return WHEEL_OK;
   }

   /*!
      Adds a resource to search path

      \return <code>WHEEL_OK</code> on success, otherwise an error code depicting the error.
   */
   uint32_t AddToPath(const string& path, const string& mountpoint)
   {
      if (PHYSFS_mount(path.std_str().c_str(), mountpoint.std_str().c_str(), 1) == 0)
         return WHEEL_RESOURCE_UNAVAILABLE;

      return WHEEL_OK;
   }

   /*!
      \return Size of a cached buffer
   */
   size_t BufferSize(const string& filename)
   {
      if (!IsCached(filename))
         return 0;

      return internal::file_cache[filename]->size();
   }
}