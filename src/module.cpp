/*!
   @file
   \brief Contains implementations for everything needed to load external modules
   \author Jari Ronkainen
*/

#include "../include/wheel_core_module.h"

#include <iostream>
#include <cassert>

namespace wheel
{
   namespace core
   {
      void ModuleLibrary::PrintAll()
      {
         for (auto pair : modules)
            std::cout << pair.first << " == " << pair.second << "\n";
      }
      ModuleLibrary::ModuleLibrary()
      {
      }
      ModuleLibrary::~ModuleLibrary()
      {
         for (auto lib : modules)
         {
            void* lib_ptr = lib.second->library_handle;

            typedef Module* (*modptr_fun_t)(Module*);

            modptr_fun_t remove_module = (modptr_fun_t) dlsym(lib_ptr, "remove_module");

            remove_module(lib.second);
            dlclose(lib_ptr);
         }
      }

      //! Adds a module file into the module library.
      /*!
         Loads a shared object or dynamically linked library file to be used as a module.

         \param   filename The filename of the library file to be loaded as a module.

         \return  <code>WHEEL_OK</code> on success, otherwise an unsigned integer depicting the error.
      */
      uint32_t ModuleLibrary::Add(const string& filename)
      {
         void* library = nullptr;
         library = dlopen(filename.c_str(), RTLD_LAZY);

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

         module->library_handle = library;

         modules.insert({filename, module});

         return WHEEL_OK;
      }

      //! Retrieves a pointer to a module from the module library.
      /*!
         \param   ident Identifier of the module in the library (usually filename)

         \return  Pointer to the module requested.
      */
      Module* ModuleLibrary::operator[](const string& ident)
      {
         return modules[ident];
      }
   }
}