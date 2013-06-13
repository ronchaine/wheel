#include "../include/wheel_core.h"
#include "../include/wheel_module_video.h"

#include <physfs.h>

#include <GL/gl.h>

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
   wheel::vertex_t tripoints[3];
   tripoints[0].x = -1.0f;
   tripoints[0].y = -1.0f;
   tripoints[0].z = 0.0f;

   tripoints[1].x = 1.0f;
   tripoints[1].y = -1.0f;
   tripoints[1].z = 0.0f;

   tripoints[2].x = 0.0f;
   tripoints[2].y = 1.0f;
   tripoints[2].z = 0.0f;

   wheel::shapes::triangle_t tri;
   tri.point[0] = tripoints[0];
   tri.point[1] = tripoints[1];
   tri.point[2] = tripoints[2];

   renderer->OpenWindow("Hello triangle", 400, 400);
   if (renderer->AddShader("default", "tri.vs", "tri.fs"))
      goto clean; // Suck it, this is clean way to end this

   if (renderer->UseShader("default") != WHEEL_OK)
      goto clean;

   renderer->Clear(0.1f, 0.01f, 0.8f, 1.0f);
 
   while(renderer->WindowIsOpen())
   {

      glBegin(GL_TRIANGLES);
         glVertex2f(-1.0f, -1.0f);
         glVertex2f(1.0f, -1.0f);
         glVertex2f(0.0f, 1.0f);
      glEnd();

      //renderer->Draw(1, &tri);
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

clean:
   modulelibrary.Remove(modlist[choice-1].file);

   wcl::terminate();

   return 0;
}