/*!
   @file
   \brief Debug class/function implementations
   \author Jari Ronkainen
*/

#include "../include/wheel_core_debug.h"

namespace wheel
{
   namespace core
   {
      std::fstream Log::out;

      Log log;

      Log::~Log()
      {
         #ifndef WHEEL_NO_DEBUG
         if (out.is_open())
            out.close();
         #endif
      }
      void Log::open(const std::string& file)
      {
         #ifndef WHEEL_NO_DEBUG
         out.open(file, std::fstream::out);
         #endif
      }
      void Log::close()
      {
         #ifndef WHEEL_NO_DEBUG
         out.close();
         #endif
      }
   }
}