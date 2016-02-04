/*!
   @file
   \brief Contains debug definitions
   \author Jari Ronkainen
*/

#ifndef WHEEL_DEBUG_HEADER
#define WHEEL_DEBUG_HEADER

#include <iostream>
#include <fstream>

#include "wheel_core_common.h"

namespace wheel
{
   class Log
   {
      template<typename T>
      friend inline wheel::Log& operator<<(wheel::Log& output, const T& stream)
      {
         #ifndef WHEEL_NO_DEBUG
            if (output.out_to_stdout())
            {
               std::cout << stream;
            } else {
               wheel::Log::out << stream;
            }
         #endif
         return output;
      }

      private:
         static std::fstream out;

      public:
         void open(const std::string& file);
         void close();

         bool out_to_stdout() { return !out.is_open(); }

        ~Log();
   };

   extern Log log;

   inline const char* get_error(uint32_t errorcode)
   {
      switch(errorcode)
      {
         case WHEEL_OK:
            return "No error";
         case WHEEL_RESOURCE_UNAVAILABLE:
            return "Resource unavailable";
         case WHEEL_INVALID_PATH:
            return "Invalid path";
         case WHEEL_UNKNOWN_FORMAT:
            return "Unknown format";
         case WHEEL_INVALID_VALUE:
            return "Invalid value";
         case WHEEL_UNEXPECTED_END_OF_FILE:
            return "Unexpected end of file";

         case WHEEL_ERROR_INIT_FILESYSTEM:
            return "Error in filesystem init";

         case WHEEL_UNABLE_TO_OPEN_MODULE:
            return "Unable to open module";
         case WHEEL_UNABLE_TO_REGISTER_MODULE:
            return "Unable to register module";
         case WHEEL_CONFLICTING_MODULE:
            return "Module is conflicting";
         case WHEEL_MISSING_DEPENDENCIES:
            return "Missing module dependencies";

         case WHEEL_UNIMPLEMENTED_FEATURE:
            return "Unimplemented feature";

         case WHEEL_MODULE_FAILED_REQUEST:
            return "Failed request";
         case WHEEL_MODULE_SHADER_COMPILE_ERROR:
            return "Failed to compile shader";
         case WHEEL_MODULE_SHADER_LINK_ERROR:
            return "Failed to link shader";

         default:
            return "Unknown error or no error message given for the value";
      }
   }
}

#endif
