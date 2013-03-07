/*!
   @file
   \brief Contains definitions for the resourcemanager class.
*/

#ifndef WHEEL_RESOURCEMANAGER_HEADER
#define WHEEL_RESOURCEMANAGER_HEADER

#include <unordered_map>
#include <physfs.h>

#include "wheel_core_string.h"

namespace wheel
{
   namespace core
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
         public:
            std::unordered_map<string, buffer_t>   library;
      };
   }
}

#ifndef WHEEL_NAMESPACE_CORE_ALIASES_DEFINED
#define WHEEL_NAMESPACE_CORE_ALIASES_DEFINED
namespace wcl = wheel::core;
#endif //WHEEL_NAMESPACE_ALIASES_DEFINED

#endif // WHEEL_RESOURCEMANAGER_HEADER