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

   int collision_count = 0;
   int total_words = 0;
   int duplicates = 0;

   while(getline(std::cin, input))
   {
      total_words++;
      wheel::string output(input);
      hash = output.hash();

      if ((it = hashlist.find(hash)) != hashlist.end())
      {
         if (it->second == output)
            duplicates++;
         else
         {
            std::cout << "Hash collision with " << output << " and " << it->second << "::" << hash << "\n";
            collision_count++;
         }
      } else {
         hashlist.insert(std::pair<size_t, wheel::string>(hash, output));
      }
   }

   std::cout << "Words checked:" << total_words << " (" << total_words - duplicates << ")  Collisions:" << collision_count << "\n";

   return collision_count;
}