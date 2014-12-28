#ifndef WHEEL_RESOURCELOADER_H
#define WHEEL_RESOURCELOADER_H

#include "../include/wheel_core_library.h"

namespace wheel
{

   // Resource hash table, static.
   std::unordered_map<wcl::string, resource_entry_t> Library::resources;

   //! Handler for loading unknown formats
   /*!
      Handler for loading unknown formats, simply puts the unmodified buffer into
      memory.
   */
   uint32_t Library::load_unknown(const wheel::string& name, const buffer_t& buffer)
   {
      wheel::log << "+ loading unknown format resource '" << name << "'\n";

      resources[name].type = WHEEL_FILE_FORMAT_UNKNOWN;
      resources[name].ptr  = new Resource(WHEEL_FILE_FORMAT_UNKNOWN, buffer);

      return WHEEL_OK;
   }

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

   Library::Library()
   {
      // Set default file handlers.

      // Unknown format handler
      file_handlers[WHEEL_FILE_FORMAT_UNKNOWN] = this->load_unknown;

      // Builtin formats.
      file_handlers[WHEEL_FILE_FORMAT_PNG];
   }

   Library::~Library()
   {
      for (auto r : resources)
      {
         if (r.second.ptr != nullptr)
            delete r.second.ptr;
      }
   }

   //!
   /*!
   */
   void Library::SetHandler(uint32_t type, std::function<uint32_t(const wheel::string&, const wheel::buffer_t&)> func)
   {
      file_handlers[type] = func;
   }

   //!
   /*!
   */
   void Library::RemoveHandler(uint32_t type)
   {
      // Do not allow unknown format handler to be deleted.
      assert(type != WHEEL_FILE_FORMAT_UNKNOWN);

      // If assertions are disabled, just fail silently.
      if (type == WHEEL_FILE_FORMAT_UNKNOWN)
         return;


   }

   uint32_t Library::Load(const wcl::string& file)
   {
      wheel::buffer_t* file_buffer = (wheel::buffer_t*)wheel::GetBuffer(file);

      uint32_t file_type = CheckFileFormat(*file_buffer);



      wheel::DeleteBuffer(file);

      return WHEEL_UNIMPLEMENTED_FEATURE;
   }
}

#endif