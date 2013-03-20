#include "../include/wheel_core.h"
#include "../include/wheel_module_video.h"

int main(int argc, char* argv[])
{
   if (argc != 2)
   {
      std::cout << "Usage: modinfo <library file>\n\n";
      return 0;
   }

   wcl::initialise(argc, argv);

   wcl::ModuleLibrary testlib;

   uint32_t err = testlib.Add(argv[1]);

   if (err)
   {
      wcl::terminate();
      std::cout << "could not open library (error " << err << ")\n";
      return 1;
   }

   wheel::core::modinfo_t modinfo;

   wheel::video::Renderer* renderer;

   renderer = (wheel::video::Renderer*)testlib[argv[1]];

   std::cout << "Loaded renderer from " << argv[1] << "\n";

   renderer->OpenWindow("Hello triangle", 800, 600);

   while(renderer->WindowIsOpen())
   {
      renderer->Update();
      renderer->SwapBuffers();
   }

   testlib.Remove(argv[1]);

   wcl::terminate();
}