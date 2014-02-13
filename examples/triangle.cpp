#include "../include/wheel_core.h"
#include "../include/wheel_module_video.h"

#include <physfs.h>
#include <cstring>

#define GL3_PROTOTYPES 1
#include <GL/gl3.h>

int main(int argc, char* argv[])
{
   wcl::initialise(argc, argv);

   wcl::ModuleLibrary modulelibrary;

   wheel::string module_search_base = wheel::AppPath();
   module_search_base += "../";

   modulelibrary.Search(module_search_base);

   wheel::modulelist_t modlist = modulelibrary.GetList("VIDEO");

   std::cout << "Available renderer modules:\n";

   int rnum = 0;
   for (auto it : modlist)
   {
      std::cout << " " << ++rnum << ") " << it.details.name << "  --  " 
                << it.details.description << "  ("
                << it.file << ")\n";
   }

   int choice = 0;

   std::cout << " Enter number: ";
   std::cin >> choice;

   if ((choice > rnum) || (choice < 1))
   {
      std::cout << "invalid renderer\n";
      wcl::terminate();
      return 1;
   }

   wheel::log << "\nLoading " << modlist[choice-1].details.name << "...\n";

   uint32_t err = modulelibrary.Add(modlist[choice-1].file);

   if (err)
   {
      wheel::log << "could not open library (error " << err << ")\n";
      wcl::terminate();
      return 1;
   }

   wheel::interface::Video* renderer = (wheel::interface::Video*)modulelibrary[modlist[choice-1].file];

   wheel::EventList eventlist;
   eventlist.clear();

   // Create triangle
//   wheel::shapes::triangle_t tri;

   renderer->OpenWindow("Hello triangle", 400, 400);

   renderer->AddShader("default", "tri.vs", "tri.fs");
   renderer->UseShader("default");

   renderer->Clear(0.1f, 0.01f, 0.01f, 1.0f);

   while(renderer->WindowIsOpen())
   {
      renderer->SwapBuffers();
      renderer->GetEvents(&eventlist);

      for(wheel::Event e : eventlist)
      {
         if (e.data.empty())
            continue;

         for (uint8_t v : e.data)
            std::cout << "[" << (int)v << "]";
         std::cout << "\n";
      }

      eventlist.clear();
   }

   modulelibrary.Remove(modlist[choice-1].file);

   wcl::terminate();

   return 0;
}
