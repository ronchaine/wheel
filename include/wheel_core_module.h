/*!
   @file
   \brief Contains definitions for everything needed to load external modules
   \author Jari Ronkainen
*/

#ifndef WHEEL_MODULE_HEADER
#define WHEEL_MODULE_HEADER

#include "wheel_core_common.h"
#include "wheel_core_string.h"

// FIXME: dlfcn is UNIX-specific, windows uses something different.
//        This should work with MingW32 though.
#include <dlfcn.h>
#include <unordered_map>
#include <set>

namespace wheel
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

      string wheel_required_version;

      friend inline std::ostream& operator<<(std::ostream& out, const wheel::modinfo_t& modinfo)
      {
         out << "---------------------\n";
         out << "Module information\n" ;
         out << "---------------------\n";
         out << "Name:" << modinfo.name << "\n";
         out << "Version:" << modinfo.version << "\n";
         out << "Type:" << modinfo.type << "\n";
         out << "Description:" << modinfo.description << "\n";

         out << "Library version requirement: " << modinfo.wheel_required_version << "\n";

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

   /*!
      \brief Virtual module class.  Derive this to add loadable modules
   */
   class Module
   {
      public:
         void* library_handle;

         virtual ~Module() {}
         virtual void get_module_info(modinfo_t*) = 0;
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

      public:
         uint32_t Add(const string& file);
         void Remove(const string& ident);

         uint32_t Search(const string& path);

         void PrintAll();

         modulelist_t GetList(const string& type = "");

         Module* operator[](const string& ident);

         ModuleLibrary();
        ~ModuleLibrary();
   };

}

#endif //WHEEL_MODULE_HEADER
