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
   }
   //! initialise the library, should NOT be called most of the time.  Use initialise(int argc, char* argv[]) instead.
   int initialise()
   {
      PHYSFS_init(NULL);

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

      internal::flags |= WHEEL_INITIALISED;

      uint16_t r = 0xbe1e;
      if (*(uint8_t*)&r == 0x1e)
         internal::flags |= WHEEL_LITTLE_ENDIAN;

      return WHEEL_OK;
   }

   void terminate()
   {
      PHYSFS_deinit();

      internal::flags &= ~WHEEL_INITIALISED;
   }
}