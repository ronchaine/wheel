#include <iostream>

#include <wheel_core.h>

namespace wheel
{
   namespace internal
   {
      extern uint32_t flags;
   }
}

int main(int argc, char* argv[])
{
   wcl::initialise(argc, argv);

   std::cout << wcl::_internal_flags << "\n\n";

   std::cout << "String unit tests" << "\n";
   wcl::string s1, s2, s3;
   s1 = U"ÄÄK!";
   s2 = U"ÖRGH!";

   std::cout << " Addition... ";
   s3 = s1 + U" " + s2;

   int result = 0;

   if (s3 == U"ÄÄK! ÖRGH!")
   {
      std::cout << "pass\n";
   } else
      result = -1;

   std::cout << " Containment... ";
 
   s1 = U"test";
   s2 = U"yeah, this is a test";
   s3 = U"Whatever";

   std::cout << "\n";

   wcl::string* s4 = new wcl::string(1234.f);
   wcl::string* s5 = new wcl::string(1235.f);

   std::string ss4("1234");
   std::string ss5("1234");

   std::hash<wcl::string> hash_fn;
   std::hash<std::string> hash_fn2;

   size_t hash4 = hash_fn(*s4);
   size_t hash5 = hash_fn(*s5);

   std::cout << std::hex << hash4 << "\n";
   std::cout << std::hex << hash5 << "\n";

   hash4 = hash_fn2(s4->c_str());
   hash5 = hash_fn2(s5->c_str());

   std::cout << std::hex << hash4 << "\n";
   std::cout << std::hex << hash5 << "\n";

   if (s1.contains(s2))
   {
      result = -1;
      std::cout << "fail\n";
   }
   if (s2.contains(s1))
      std::cout << "pass\n";

   return result;
}