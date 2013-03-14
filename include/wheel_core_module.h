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
            virtual ~Module() {}
            virtual void ModInfo(modinfo_t&) = 0;

            std::unordered_map<string, void*> functionmap;

            virtual uint32_t register_module() = 0;
            virtual uint32_t remove_module() = 0;
      };

      /*!
         \brief Library manager for loaded modules
      */
      class ModuleLibrary
      {
         private:
            std::vector<string> searchpath;

         public:
            std::unordered_map<string, Module*> module;

            int32_t Add(string file);
            int32_t Remove(string file);

            string List(string type);

            ModuleLibrary();
           ~ModuleLibrary();
      };
   }
}

#endif //WHEEL_MODULE_HEADER
