/*!
   @file
   \brief Contains definitions for the resourcemanager class.
   \author Jari Ronkainen
*/

#ifndef WHEEL_RESOURCEMANAGER_HEADER
#define WHEEL_RESOURCEMANAGER_HEADER

#include <unordered_map>
#include <physfs.h>

#define W_PHYSFS_BEGIN           0
#define W_PHYSFS_END             1

#include "wheel_core_common.h"
#include "wheel_core_string.h"

namespace wheel
{
   //! Struct to hold buffered data.
   /*!
   */
   struct buffer_t
   {
      size_t   size;
      uint8_t* data;
      size_t   pos;

      buffer_t() {
         size = 0;
         data = nullptr;
         pos = 0;
      }
      ~buffer_t() {
         if (data != nullptr)
            free(data);
      }
   };

   //! A collection of resources
   /*!
      Handles loading resources into memory.
   */
   class ResourceManager
   {
      private:
         static uint32_t resourcemanager_count;

         std::unordered_map<string, buffer_t>      library;
         std::unordered_map<string, PHYSFS_file*>  files;

      public:
         buffer_t*      BufferFile(const string& file);
         void           FreeFile(const string& file);

         buffer_t*      RetrieveBuffer(const string& file);
         size_t         BufferedSize(const string& file);

         uint32_t       AddResource(const string& file);
         uint32_t       RemoveResource(const string& file);

         uint32_t       OpenFile(const string& file);
         uint32_t       Close(const string& file);

         PHYSFS_file*   GetHandle(const string& file);

         ResourceManager();
        ~ResourceManager();
   };
}

#ifndef WHEEL_NAMESPACE_CORE_ALIASES_DEFINED
#define WHEEL_NAMESPACE_CORE_ALIASES_DEFINED
namespace wcl = wheel;
#endif //WHEEL_NAMESPACE_ALIASES_DEFINED

#endif // WHEEL_RESOURCEMANAGER_HEADER