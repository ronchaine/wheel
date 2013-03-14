/*!
   @file
   \brief Contains required functions for the wheel to work.
   \author Jari Ronkainen
*/

#include "../include/wheel_core.h"

#include <physfs.h>

namespace wheel
{
   namespace core
   {
      uint32_t _internal_flags = 0;

      //! initialise the library, should NOT be called most of the time.  Use initialise(int argc, char* argv[]) instead.
      int initialise()
      {
         PHYSFS_init(NULL);

         return 0;
      }

      //! initialise the library.
      /*!
         Initialise the library.  Should be called before any library functions are used.
         It should be passed the arguments from <code>main</code> function.
      */
      int initialise(int argc, char* argv[])
      {
         PHYSFS_init(argv[0]);

         _internal_flags |= WHEEL_INITIALISED;

         uint16_t r = 0xbe1e;
         if (*(uint8_t*)&r == 0x1e)
            _internal_flags |= WHEEL_LITTLE_ENDIAN;

         return 0;
      }

      void terminate()
      {
         PHYSFS_deinit();

         _internal_flags &= ~WHEEL_INITIALISED;
      }
   }
}