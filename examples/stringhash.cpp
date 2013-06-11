#include "../include/wheel_core.h"

#include <iostream>
#include <string>
#include <map>

int main(void)
{
   std::string input;

   std::map<size_t, wheel::string> hashlist;
   std::map<size_t, wheel::string>::iterator it;

   size_t hash;

   while(getline(std::cin, input))
   {

      wheel::string output(input);
      hash = output.hash();

      if ((it = hashlist.find(hash)) != hashlist.end())
      {
         if (it->second == output)
            std::cout << "Duplicate entry '" << output << "'\n";
         else
            std::cout << "Hash collision with " << output << " and " << it->second << "::" << hash << "\n";
      } else {
         hashlist.insert(std::pair<size_t, wheel::string>(hash, output));
      }
   }

   return 0;
}