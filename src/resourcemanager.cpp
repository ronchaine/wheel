/*!
   @file
   \brief Contains implementations for the resourcemanager class.
   
   Resource manager uses <i>physfs</i> to create virtual filesystem from which
   the files can be loaded.

   \author Jari Ronkainen
*/

#include "../include/wheel_core_resourcemanager.h"

namespace wheel
{
   namespace core
   {
      uint32_t ResourceManager::resourcemanager_count = 0;

      //! Loads file from virtual filesystem into memory
      /*!
      */
      buffer_t* ResourceManager::BufferFile(const string& file)
      {
         return nullptr;
      }

      //! Adds a compressed file or a directory to the virtual filesystem
      /*!
         Adds a archive or file the name of which is given as an argument to the virtual filesystem,
         \param  resource  Name of the archive or directory to add to the virtual filesystem.
         \return <code>WHEEL_OK</code> on success, otherwise an error code describing the error.
      */
      uint32_t ResourceManager::AddResource(const string& resource)
      {
         if (PHYSFS_addToSearchPath(resource.c_str(), W_PHYSFS_BEGIN) != 0)
            return WHEEL_OK;

         return WHEEL_RESOURCE_UNAVAILABLE;
      }

      //! Removes a resource from the virtual filesystem
      /*!
      */
      uint32_t ResourceManager::RemoveResource(const string& resource)
      {
         return WHEEL_OK;
      }

      ResourceManager::ResourceManager()
      {
      }

      ResourceManager::~ResourceManager()
      {
      }
   }
}