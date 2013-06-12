#include "../include/wheel_core.h"
#include "../include/wheel_module_video.h"

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

   wheel::modinfo_t modinfo;

   renderer->get_module_info(&modinfo);

   wheel::log << modinfo << "\n";

   wheel::EventList eventlist;
   eventlist.clear();

   renderer->OpenWindow("Hello triangle", 200, 200);

   renderer->Clear(0.1f, 0.01f, 0.1f, 1.0f);
//   renderer->Draw(1, &tri);

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