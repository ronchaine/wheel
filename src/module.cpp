/*!
   @file
   \brief Contains implementations for everything needed to load external modules
*/

#include "../include/wheel_core_module.h"

//wheel::core::Module* add_module();

namespace wheel
{
   namespace core
   {
      ModuleLibrary::ModuleLibrary()
      {
      }
      ModuleLibrary::~ModuleLibrary()
      {
      }

      uint32_t ModuleLibrary::Add(const string& filename)
      {
         void* library;
         dlopen(filename.c_str(), RTLD_LAZY);

         if (!library)
         {
            return WHEEL_UNABLE_TO_OPEN_MODULE;
         }

         dlerror();

         typedef Module* (*modptr_fun_t)();

         modptr_fun_t add_module = (modptr_fun_t) dlsym(library, "register_module");

         const char *dlsym_err = dlerror();
         if (dlsym_err)
         {
            dlclose(library);
            return WHEEL_UNABLE_TO_REGISTER_MODULE;
         }

         Module* module = add_module();

         std::pair<string, Module*> pair(filename, module);

         modules.insert(pair);

         dlclose(library);
      }
   }
}