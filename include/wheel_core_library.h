#ifndef WHEEL_RESOURCE_LOADER_H
#define WHEEL_RESOURCE_LOADER_H

#include "wheel_core_common.h"
#include "wheel_core_resource.h"

namespace wheel
{
   struct resource_entry_t
   {
      uint32_t    type;
      Resource*   ptr;
   };

   class Library
   {
      private:
         static std::unordered_map<wcl::string, resource_entry_t> resources;
         std::unordered_map<uint32_t, std::function<uint32_t(const wheel::string&, const wheel::buffer_t&)>> file_handlers;

         static uint32_t   instance_count;

         static uint32_t   load_unknown(const string& entry, const buffer_t& buffer);

      public:
         static uint32_t   AddBuffer(uint32_t type, const string& name, const buffer_t&);

         uint32_t          Load(const wcl::string& file);

         void              SetHandler(uint32_t type, std::function<uint32_t(const wheel::string&, const wheel::buffer_t&)> func);
         void              RemoveHandler(uint32_t type);

         Library();
        ~Library();
   };
}

#endif