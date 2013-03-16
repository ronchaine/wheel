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
         if (out.is_open())
            out.close();
      }
      void Log::open(const std::string& file)
      {
         out.open(file, std::fstream::out);
      }
      void Log::close()
      {
         out.close();
      }
   }
}