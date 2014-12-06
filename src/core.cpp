/*!
   @file
   \brief Contains required functions for the wheel to work.
   \author Jari Ronkainen
*/

#include <wheel_core.h>
#include <wheel_core_debug.h>

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

   //! initialise the library.
   /*!
      Initialise the library.  Should be called before any library functions are used.
      It should be passed the arguments from <code>main</code> function.
   */
   int initialise(int argc, char* argv[])
   {
      if (Filesystem_Init(argc, argv))
         return WHEEL_ERROR_INIT_FILESYSTEM;

      uint32_t res = internal::init();
      if (res != WHEEL_OK)
         return res;

      return WHEEL_OK;
   }

   //! Uninitialize the library
   /*!
      Cleans up after the library.
   */
   void terminate()
   {
      Filesystem_Deinit();

      internal::flags &= ~WHEEL_INITIALISED;
   }
}