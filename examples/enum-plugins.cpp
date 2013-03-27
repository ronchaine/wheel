#include "../include/wheel_core.h"

int main(int argc, char* argv[])
{
   if ((argc != 2) && (argc != 3))
   {
      std::cout << "Usage: modenum <path> [type]\n\n";
      return 0;
   }

   wcl::initialise(argc, argv);

   wcl::ModuleLibrary testlib;

   std::cout << "Searching " << argv[1] << "\n";

   testlib.Search(argv[1]);

   wheel::string searchtype;
   if (argc == 3)
      searchtype = argv[2];
   else
      searchtype = "";  // Lists all modules

   wheel::modulelist_t modlist = testlib.GetList(searchtype);

   if (searchtype.empty())
      std::cout << "Modules found:\n";
   else
      std::cout << "Modules of type \"" << searchtype << "\" found:\n";

   for (auto it : modlist)
   {
      std::cout << "  " << it.details.name << "  --  " 
                << it.details.description << "  ("
                << it.file << ")\n";
   }

   wcl::terminate();
}