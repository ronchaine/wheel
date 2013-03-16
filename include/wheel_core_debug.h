/*!
   @file
   \brief Contains debug definitions
   \author Jari Ronkainen
*/

#ifndef WHEEL_DEBUG_HEADER
#define WHEEL_DEBUG_HEADER

#include <iostream>
#include <fstream>

namespace wheel
{
   namespace core
   {
      class Log
      {
         template<typename T>
         friend wheel::core::Log& operator<<(wheel::core::Log& output, const T& stream)
         {
            #ifndef WHEEL_NO_DEBUG
               if (output.out_to_stdout())
               {
                  std::cout << stream;
               } else {
                  wheel::core::Log::out << stream;
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
   }
}

#endif