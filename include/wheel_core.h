/*!
   @file
   \brief Contains core definitions
   \author Jari Ronkainen
*/

#ifndef WHEEL_CORE_HEADER
#define WHEEL_CORE_HEADER

#include "wheel_core_common.h"
#include "wheel_core_string.h"
#include "wheel_core_utility.h"
#include "wheel_core_resourcemanager.h"
#include "wheel_core_module.h"
#include "wheel_core_debug.h"

namespace wheel
{
   namespace core
   {
      int   initialise(int argc, char* argv[]);
      void  terminate();
   }
}

#endif