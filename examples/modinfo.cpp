#include "../include/wheel_core.h"

int main(int argc, char* argv[])
{
   wcl::initialise(argc, argv);

   wcl::ModuleLibrary testlib;

   testlib.Add(argv[1]);
   wcl::terminate();
}