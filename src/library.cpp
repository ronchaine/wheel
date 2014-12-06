#ifndef WHEEL_RESOURCELOADER_H
#define WHEEL_RESOURCELOADER_H

#include <wheel_library.h>

namespace wheel
{
   // ============================================================================

   //! Load a known resource type from a file
   /*!
      Loads a file to memory buffer for later use

      \param file  filename to load
   */
   Resource* Resource::Load(const wcl::string& file)
   {
      wheel::log << "loading file '" << file << "'\n";

      Resource* new_resource = nullptr;

      wheel::buffer_t* file_buffer = (wheel::buffer_t*)wheel::GetBuffer(file);

      if (file_buffer == nullptr)
         return nullptr;

      uint32_t file_format = CheckFileFormat(*file_buffer);
/*
      if (file_format == WHEEL_FILE_FORMAT_PNG)
      {}

      wheel::DeleteBuffer(file);

*/
      return new_resource;
   }

   Resource* Library::Load(const wcl::string& file)
   {
      return nullptr;
   }
}

#endif