/*!
   @file
   \brief Contains definitions for everything needed to load external modules
*/

#ifndef WHEEL_MODULE_HEADER
#define WHEEL_MODULE_HEADER

#include "wheel_core_common.h"
#include "wheel_core_string.h"

// FIXME: dlfcn is UNIX-specific, windows uses something different.
//        This should work with MingW32 though.
#include <dlfcn.h>
#include <unordered_map>

namespace wheel
{
   namespace video
   {
      class Renderer
      {
      };
   }
}

namespace wheel
{
   namespace core
   {
      /*!
         \brief Structure that holds runtime information about the module.
      */
      struct modinfo_t
      {
         string type;
         string name;
         string version;
         string description;
      };

      /*!
         \brief Virtual module class.  Derive this to add loadable modules
      */
      class Module
      {
         public:
            void* handle;

            virtual ~Module() {}
            virtual void ModInfo(modinfo_t&) = 0;

            virtual uint32_t register_module();
            virtual uint32_t remove_module();
      };

      /*!
         \brief Library manager for loaded modules
      */
      class ModuleLibrary
      {
         private:
            std::vector<string> searchpath;
            std::unordered_map<string, Module*> modules;

         public:

            uint32_t Add(const string& file);
            uint32_t Remove(const string& ident);

            string List(const string& type);

            Module* operator[](const string& ident);

            ModuleLibrary();
           ~ModuleLibrary();
      };
   }
}

#endif //WHEEL_MODULE_HEADER
