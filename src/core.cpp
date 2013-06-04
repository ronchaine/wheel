/*!
   @file
   \brief Contains required functions for the wheel to work.
   \author Jari Ronkainen
*/

#include "../include/wheel_core.h"
#include "../include/wheel_core_debug.h"

#include <physfs.h>
   
namespace wheel
{
   namespace internal
   {
      uint32_t flags = 0;

      int init()
      {
         internal::flags |= WHEEL_INITIALISED;

         return 0;
      }

   }

   //! initialise the library, should NOT be called most of the time.  Use initialise(int argc, char* argv[]) instead.
   int initialise()
   {
      PHYSFS_init(NULL);

      uint32_t res = internal::init();
      if (res != WHEEL_OK)
         return res;

      return WHEEL_OK;
   }

   //! initialise the library.
   /*!
      Initialise the library.  Should be called before any library functions are used.
      It should be passed the arguments from <code>main</code> function.
   */
   int initialise(int argc, char* argv[])
   {
      PHYSFS_init(argv[0]);

      uint32_t res = internal::init();
      if (res != WHEEL_OK)
         return res;

      return WHEEL_OK;
   }

   void terminate()
   {
      PHYSFS_deinit();

      internal::flags &= ~WHEEL_INITIALISED;
   }
}