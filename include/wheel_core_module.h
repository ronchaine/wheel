/*!
   @file
   \brief Contains definitions for everything needed to load external modules
   \author Jari Ronkainen
*/

// .so .dll .dylib

#ifndef WHEEL_MODULE_HEADER
#define WHEEL_MODULE_HEADER

#include "wheel_core_common.h"
#include "wheel_core_string.h"
#include "wheel_core_event.h"

#ifndef _WIN32
#include <dlfcn.h>

typedef void* module_handle_t;

#else
#include <windows.h>

// Get rid of moronic defines
#ifdef interface
   #undef interface
#endif
  
typedef HMODULE module_handle_t;

#define RTLD_NOW     0
#define RTLD_LAZY    0
#define RTLD_LOCAL   0

// Create wrappers for dlopen, dlsym, dlclose
inline module_handle_t dlopen(const char* filename, uint32_t dummy) { return LoadLibrary(filename); }
inline FARPROC dlsym(module_handle_t library, const char* func) { return GetProcAddress(library, func); }
inline const char* dlerror() { return "module error"; }
inline void dlclose(module_handle_t lib) { FreeLibrary(lib); }

#endif

#include <unordered_map>
#include <set>
#include <list>

//! Macro to make writing modules a bit cleaner
/*!
   This is used to in module files to provide needed functions for modules.
   Use name of the module class as an argument or just write the needed functions
   yourself if you need more detailed options.
*/
#define WHEEL_MODULE_REGISTER(x)                   \
   extern "C" {                                    \
      wheel::Module* register_module()             \
      {                                            \
         return new x;                             \
      }                                            \
                                                   \
      void remove_module(wheel::Module* mod)       \
      {                                            \
         delete mod;                               \
      }                                            \
   }

namespace wheel
{
   //! \brief Structure that holds runtime information about the module.
   /*!
      Holds runtime info about the module.  A module must have a function get_module_info(modinfo_t*)
      that fills this kind of struct with correct information about the module.
   */
   struct modinfo_t
   {
      string type;
      string name;
      string version;
      string description;

      string depends;
      string provides;

      string wheel_required_version;

      friend inline std::ostream& operator<<(std::ostream& out, const wheel::modinfo_t& modinfo)
      {
         out << (std::string)"---------------------\n";
         out << (std::string)"Module information\n" ;
         out << (std::string)"---------------------\n";
         out << (std::string)"Name:" << modinfo.name << (std::string)"\n";
         out << (std::string)"Version:" << modinfo.version << (std::string)"\n";
         out << (std::string)"Type:" << modinfo.type << (std::string)"\n";
         out << (std::string)"Description:" << modinfo.description << (std::string)"\n";

         out << (std::string)"Depends on:" << modinfo.depends << (std::string)"\n";
         out << (std::string)"Provides:" << modinfo.provides << (std::string)"\n";

         out << (std::string)"Library version requirement: " << modinfo.wheel_required_version << (std::string)"\n";

         return out;
      }
   };

   struct modset_t
   {
      modinfo_t   details;
      string      file;

      bool        is_loaded;

      modset_t() { is_loaded = false; }

      friend inline bool operator<(const modset_t& one, const modset_t& other)
      {
         if (one.details.type < other.details.type)
            return true;
         if (one.details.name < other.details.name)
            return true;
         if (one.details.version < other.details.version)
            return true;

         return false;
      }
   };

   typedef std::vector<modset_t> modulelist_t;

   //! \brief Virtual module class.  Derive this to add loadable modules.
   /*!
      Loadable modules must provide get_module_info that fills modinfo_t* -type
      struct with information about the module itself.

      \sa modinfo_t
   */
   class Module
   {
      private:
      protected:
      public:
         virtual ~Module() {}

         module_handle_t library_handle;

         virtual void get_module_info(modinfo_t*) = 0;

         virtual uint32_t GetEvents(EventList* events) { return WHEEL_UNIMPLEMENTED_FEATURE; }
   };

   /*!
      \brief Library manager for loaded modules
   */
   class ModuleLibrary
   {
      private:
         std::vector<string> searchpath;
         std::unordered_map<string, Module*> modules;

         std::set<modset_t> known_modules;

         std::vector<string> provided;

      public:
         uint32_t Add(const string& file);
         void Remove(const string& ident);

         uint32_t Search(const string& path);

         void PrintAll();

         modulelist_t GetList(const string& type = "") const;

         Module* operator[](const string& ident);

         ModuleLibrary();
        ~ModuleLibrary();
   };
}

#endif //WHEEL_MODULE_HEADER
