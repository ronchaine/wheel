#include "../include/wheel_core.h"
#include "../include/wheel_module_video.h"
#include "../include/wheel_math_geometry.hpp"

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

//   renderer->SetWindowHints(GL);

   wheel::EventList eventlist;
   eventlist.clear();

   wmath::vec3f r[3], p[3];
   wmath::vec4f c[3];  

   r[0] = {0.0f, 0.0f, 0.0f};          // position
   p[0] = {0.0f, 0.0f, 0.0f};          // normal
   c[0] = {1.0f, 0.0f, 0.0f, 1.0f};    // colour

   r[1] = {0.5f, 0.5f, 0.0f};          // position
   p[1] = {0.0f, 0.0f, 0.0f};          // normal
   c[1] = {1.0f, 0.0f, 0.0f, 1.0f};    // colour

   r[2] = {0.5f,-0.5f, 0.0f};          // position
   p[2] = {0.0f, 0.0f, 0.0f};          // normal
   c[2] = {1.0f, 0.0f, 0.0f, 1.0f};    // colour

   wheel::shapes::triangle_t tri;

   tri.point[0] = wheel::shapes::vertex_t(r[0], p[0], c[0]);
   tri.point[0] = wheel::shapes::vertex_t(r[1], p[1], c[1]);
   tri.point[0] = wheel::shapes::vertex_t(r[2], p[2], c[2]);

//   wheel::library.AddToPath(".");

   renderer->OpenWindow("Hello triangle", 200, 200);

   renderer->Clear(0.1f, 0.01f, 0.1f, 1.0f);
   renderer->Draw(1, &tri);

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