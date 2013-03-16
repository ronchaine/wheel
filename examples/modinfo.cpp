#include "../include/wheel_core.h"

int main(int argc, char* argv[])
{
   if (argc != 2)
   {
      std::cout << "Usage: modinfo <library file>\n\n";
      return 0;
   }

   wcl::initialise(argc, argv);

   wcl::ModuleLibrary testlib;

   std::cout << "trying file " << argv[1] << "\n";

   uint32_t err = testlib.Add(argv[1]);

   if (err)
   {
      wcl::terminate();
      std::cout << "could not open library\n";
      return 1;
   }

   wheel::core::modinfo_t modinfo;

   testlib[argv[1]]->get_module_info(&modinfo);

   wheel::core::log.open("out.log");
   wheel::core::log << modinfo << "\n";
   wheel::core::log.close();

   wcl::terminate();
}