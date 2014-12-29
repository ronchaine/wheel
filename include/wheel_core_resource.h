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
   //! Resource, parent class for all loadable resources.
   /*!
      
   */
   class Resource
   {
      protected:
         uint32_t format;
         buffer_t data;

      public:
         Resource() : format(WHEEL_FILE_FORMAT_UNKNOWN) {}
         Resource(wheel_resource_t type, const buffer_t& buf);

         static Resource* Load(const wheel::string& file);

         inline buffer_t* data_ptr() { return &data; }
   };

   const char*       AppPath();
   const char*       UserPath();

   uint32_t          CreateDirectory(const char* dir);

   uint32_t          SetWritePath(const char* newpath);
   void              ShowSearchPath();

   uint32_t          Filesystem_Init(int argc, char* argv[]);
   void              Filesystem_Deinit();

   uint32_t          AddToPath(const string& resource, const string& where);

   const buffer_t*   GetBuffer(const string& filename);
   size_t            BufferSize(const string& filename);

   bool              IsCached(const string& filename);

   uint32_t          Buffer(const string& filename);
   void              DeleteBuffer(const string& filename);

   void              EmptyCache();

   uint32_t          WriteBuffer(const string& file, const buffer_t& buffer);
}

#endif