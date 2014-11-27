#ifndef WHEEL_RESOURCE_LOADER_H
#define WHEEL_RESOURCE_LOADER_H

#include "wheel_core_resource.h"
#include "wheel_image_common.h"
#include "wheel_image_png.h"

namespace wheel
{
   class Library
   {
      private:
         std::unordered_map<wcl::string, Resource*>   library;

      public:
         Resource* Load(const wcl::string& file);

         Library();
        ~Library();
   };
}

#endif