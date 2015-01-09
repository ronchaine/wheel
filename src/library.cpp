#ifndef WHEEL_RESOURCELOADER_H
#define WHEEL_RESOURCELOADER_H

#include "../include/wheel_core_library.h"
#include "../include/wheel_image.h"

namespace wheel
{

   // Resource hash table, static.
   std::unordered_map<wcl::string, resource_entry_t> Library::resources;

   // Count of library instances
   uint32_t Library::instance_count = 0;

   //! Handler for loading unknown formats
   /*!
      Handler for loading unknown formats, simply puts the unmodified buffer into
      memory.
   */
   uint32_t Library::load_unknown(const wheel::string& name, buffer_t& buffer)
   {
      wheel::log << "+ loading unknown format resource '" << name << "'\n";
      AddBuffer(WHEEL_RESOURCE_RAW, name, buffer);

      return WHEEL_OK;
   }

   void Library::unload_resource(resource_entry_t r)
   {
      if (r.type == WHEEL_RESOURCE_IMAGE)
         delete (wheel::image::Image*)r.ptr;
      else
         delete r.ptr;
   }

   //! Add a buffer to resources
   /*!
      Static function to add resources to library.

      \param   type     Type of the buffer, must match one of the enums from wheel_resource_t
      \param   name     Name to call the buffer in the library
      \param   buffer   Buffer data.

      \return  WHEEL_OK on success, otherwise wheel error
   */
   uint32_t Library::AddBuffer(wheel_resource_t type, const wheel::string& name, const buffer_t& buffer)
   {
      // Make sure there is an instance of a library
      if (instance_count == 0)
         return WHEEL_UNINITIALISED_RESOURCE;

      // If there already is a resource with the same name, free it from memory.
      if (resources.count(name))
      {
         if (resources[name].ptr != nullptr)
            delete resources[name].ptr;
      }

      // Then just put new stuff in.
      resources[name].type = type;
      resources[name].ptr = new Resource(type, buffer);

      return WHEEL_OK;
   }

   //! Add a resource
   /*!
      Static function to add resources to library.

      \param   type     Type of the buffer, must match one of the enums from wheel_resource_t
      \param   name     Name to call the buffer in the library
      \param   rptr     Pointer to the resource

      \return  WHEEL_OK on success, otherwise wheel error
   */
   uint32_t Library::AddResource(wheel_resource_t type, const wheel::string& name, Resource* rptr)
   {
      // Make sure there is an instance of a library
      if (instance_count == 0)
         return WHEEL_UNINITIALISED_RESOURCE;

      // If there already is a resource with the same name, free it from memory.
      if (resources.count(name))
      {
         if (resources[name].ptr != nullptr)
            delete resources[name].ptr;
      }

      // Then just put new stuff in.
      resources[name].type = type;
      resources[name].ptr = rptr;

      return WHEEL_OK;
   }

   // ============================================================================

   Resource* Library::operator[](const string& name)
   {
      if (resources.count(name) != 1)
         return nullptr;

      return resources[name].ptr;
   }

   Library::Library()
   {
      // Add instance count
      instance_count++;

      // Set default file handlers.

      // Unknown format handler
      file_handlers[WHEEL_FILE_FORMAT_UNKNOWN] = this->load_unknown;

      // Builtin formats.
      file_handlers[WHEEL_FILE_FORMAT_PNG] = wheel::image::decode_png;
   }

   Library::~Library()
   {
      // Decrease instance count
      instance_count--;

      // If no instances remaining, free resources.
      if (instance_count == 0)
         for (auto r : resources)
            unload_resource(r.second);
   }

   //! Set handler for a file format
   /*!
   */
   void Library::SetHandler(wheel_filetype_t type, std::function<uint32_t(const wheel::string&, wheel::buffer_t&)> func)
   {
      file_handlers[type] = func;
   }

   //! Remove a handler for a file format
   /*!
   */
   void Library::RemoveHandler(wheel_filetype_t type)
   {
      // Do not allow unknown format handler to be deleted.
      assert(type != WHEEL_FILE_FORMAT_UNKNOWN);

      // If assertions are disabled, just fail silently.
      if (type == WHEEL_FILE_FORMAT_UNKNOWN)
         return;

      // Remove handler
      file_handlers.erase(type);
   }

   void Library::debug_listfiles()
   {
      std::cout << "::debug:: listing files in library.\n";
      for (auto r : resources)
      {
         std::cout << r.first << "\n";
      }
   }

   //! Load a file into resource library
   /*!
   */
   uint32_t Library::Load(const wcl::string& file)
   {
      wheel::buffer_t* file_buffer = (wheel::buffer_t*)wheel::GetBuffer(file);
         if (file_buffer == nullptr)
            return WHEEL_RESOURCE_UNAVAILABLE;

      uint32_t file_type = CheckFileFormat(*file_buffer);

      uint32_t rval = WHEEL_UNINITIALISED_RESOURCE;

      // If there is registered handler for the file type, use it
      if (file_handlers.count(file_type))
         rval = file_handlers[file_type](file, *file_buffer);
      else
         rval = file_handlers[WHEEL_FILE_FORMAT_UNKNOWN](file, *file_buffer);

      // We don't want to keep the original buffer.
      wheel::DeleteBuffer(file);

      return rval;
   }

   //! Unload a file from resource library
   /*!
   */
   uint32_t Library::Unload(const wcl::string& file)
   {
//         static std::unordered_map<wcl::string, resource_entry_t> resources;
      if (resources.count(file) == 0)
         return WHEEL_UNINITIALISED_RESOURCE;

      unload_resource(resources[file]);
      resources.erase(file);

      return WHEEL_OK;
   }
}

#endif