/*!
   @file
   \brief Contains implementations for everything needed to load external modules
   \author Jari Ronkainen
*/

#include <wheel_core_module.h>
#include <wheel_core_debug.h>

#include <iostream>
#include <cassert>
// FIXME: dirent is UNIX-specific, windows uses something different.
//        This should work with MingW32 though.
#include <dirent.h>

namespace wheel
{
   namespace internal
   {
      // Checks if a file is a wheel module
      bool Module_Check_Load(const string& filename, modinfo_t* save_info = nullptr)
      {
         module_handle_t library = nullptr;

         library = dlopen(filename.std_str().c_str(), RTLD_NOW | RTLD_LOCAL);

         if (!library)
         {
            wheel::log << dlerror() << "\n";
            return false;
         }
//         wheel::log << "Trying " << filename << "\n";

         dlerror();

         typedef Module* (*modptr_fun_add_t)();
         typedef void (*modptr_fun_del_t)(Module*);

         modptr_fun_add_t add_module = (modptr_fun_add_t) dlsym(library, "register_module");
         const char *dlsym_err = dlerror();
         if (dlsym_err)
         {
            dlclose(library);
            return false;
         }

         dlerror();

         modptr_fun_del_t remove_module = (modptr_fun_del_t) dlsym(library, "remove_module");
         const char *dlsym2_err = dlerror();
         if (dlsym2_err)
         {
            dlclose(library);
            return false;
         }

         if (save_info == nullptr)
         {
            dlclose(library);
            return true;
         }

         Module* temp = add_module();

         temp->get_module_info(save_info);

         remove_module(temp);

         return true;
      }
   }

   ModuleLibrary::ModuleLibrary()
   {
   }
   ModuleLibrary::~ModuleLibrary()
   {
      for (auto lib : modules)
      {
         module_handle_t lib_ptr = lib.second->library_handle;

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
      module_handle_t library = nullptr;
      library = dlopen(filename.std_str().c_str(), RTLD_NOW | RTLD_LOCAL);

      if (!library)
      {
         wheel::log << dlerror() << "\n";
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

      modinfo_t info;
      module->get_module_info(&info);

      std::vector<string> moddeps, modprovs;     
      moddeps = info.depends.split(",");
      modprovs = info.provides.split(",");

      // Check if equivalent system already exists.
      for (string s : modprovs)
         for (string s2 : provided)
         {
            if (s == s2)
            {
               typedef Module* (*modptr_r_fun_t)(Module*);
               modptr_r_fun_t remove_module = (modptr_r_fun_t) dlsym(library, "remove_module");

               remove_module(module);

               dlclose(library);
               log << "conflicted " << s << " modules\n";

               return WHEEL_CONFLICTING_MODULE;
            }
         }

      // Check for dependencies
      for (string s : provided)
         for (auto it = moddeps.begin(); it != moddeps.end(); ++it)
            if (s == *it)
            {
               moddeps.erase(it);
            }

      if (moddeps.size() > 0)
      {
         for (string s : moddeps)
            log << "missing deps:" << s << "\n";

         typedef Module* (*modptr_r_fun_t)(Module*);
         modptr_r_fun_t remove_module = (modptr_r_fun_t) dlsym(library, "remove_module");

         remove_module(module);

         dlclose(library);

         return WHEEL_MISSING_DEPENDENCIES;
      }

      modules.insert({filename, module});

      return WHEEL_OK;
   }
   void ModuleLibrary::Remove(const string& ident)
   {
      for (auto it = modules.begin(); it != modules.end(); ++it)
      {
         if (it->first != ident)
            continue;

         module_handle_t lib_ptr = it->second->library_handle;

         typedef Module* (*modptr_fun_t)(Module*);

         modptr_fun_t remove_module = (modptr_fun_t) dlsym(lib_ptr, "remove_module");

         remove_module(it->second);
         dlclose(lib_ptr);

         modules.erase(it);
         break;
      }
   }

   uint32_t ModuleLibrary::Search(const string& path)
   {
      DIR* dir;

      struct dirent* loc;

      if ((dir = opendir(path.std_str().c_str())) == nullptr)
         return WHEEL_INVALID_PATH;

      while((loc = readdir(dir)))
      {
         string filename(loc->d_name);
         if ((filename == ".") || (filename == ".."))
            continue;

         if ((filename.contains(".so")) || (filename.contains(".dll")))
         {
            modset_t info;
            if (internal::Module_Check_Load(path + "/" + filename, &info.details))
            {
               info.file = path + "/" + filename;

               auto rval = known_modules.insert(info);

               if (!rval.second)
                  wheel::log << "Warning: Found duplicate module \""
                             << info.file
                             << "\". (this may be caused by symlinks)\n";

            }
         } else
            Search(path + "/" + loc->d_name);
      }

      closedir(dir);

      return WHEEL_OK;
   }

   //! Retrieves a pointer to a module from the module library.
   /*!
      \param   ident Identifier of the module in the library (usually filename)

      \return  Pointer to the module requested.
   */
   modulelist_t ModuleLibrary::GetList(const string& type) const
   {
      modulelist_t rval;

      rval.clear();

      for (auto info : known_modules)
      {
         if ((type == "") || (info.details.type == type))
            rval.push_back(info);
      }

      return rval;
   }

   //! Retrieves a pointer to a module from the module library.
   /*!
      \param   ident Identifier of the module in the library (usually filename)

      \return  Pointer to the module requested.
   */
   Module* ModuleLibrary::operator[](const string& ident)
   {
      if (modules.count(ident) == 0)
         return nullptr;

      return modules[ident];
   }
}