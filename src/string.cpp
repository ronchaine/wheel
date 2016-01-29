/*!
   @file
   \brief Contains implementations for the string class and string manipulation functions.
   \author Jari Ronkainen
*/

#include <wheel_core_string.h>

#include "utf8.h"

#include <utility>
#include <cstdlib>
#include <cstring>

#include <iostream>

#include <cassert>

namespace wheel
{
   //! Default constructor.  Does nothing at the moment.
   string::string()
   {
   }

   //! Default copy constructor.
   string::string(const string& other)
   {
      // Use the assignment operator overload
      *this = other;
   }

   //! Default move constructor.
   string::string(string&& other)
   {
      // Uses operator= other
      *this = std::move(other);
   }

   //! Create from integer value
   /*!
      Creates a wcl string from an integer.
   */
   string::string(int64_t in)
   {
      char integer[32];

      snprintf(integer, 15, "%ld", in);
      std::string result;

      utf8::replace_invalid(integer, integer + strlen(integer), back_inserter(result));
      utf8::utf8to32(result.begin(), result.end(), back_inserter(data));
   }
   string::string(int32_t in)
   {
      char integer[32];

      snprintf(integer, 15, "%d", in);
      std::string result;

      utf8::replace_invalid(integer, integer + strlen(integer), back_inserter(result));
      utf8::utf8to32(result.begin(), result.end(), back_inserter(data));
   }

   //! Create from integer value
   /*!
      Creates a wcl string from an integer.
   */
   string::string(uint32_t in)
   {
      char integer[32];

      snprintf(integer, 15, "%d", in);
      std::string result;

      utf8::replace_invalid(integer, integer + strlen(integer), back_inserter(result));
      utf8::utf8to32(result.begin(), result.end(), back_inserter(data));
   }

   //! Create from floating point value
   /*!
      Creates a wcl string from an double-precision float.
   */
   string::string(double in)
   {
      char floatingpoint[32];

      snprintf(floatingpoint, 15, "%f", in);
      std::string result;

      utf8::replace_invalid(floatingpoint, floatingpoint + strlen(floatingpoint), back_inserter(result));
      utf8::utf8to32(result.begin(), result.end(), back_inserter(data));
   }

   //! Create from c++11 char32_t array
   /*!
      Creates a wcl string instance from data provided in C++11 char32_t
      array.  (i.e. U("This is an example."))  It is presumed that the
      character array is already UTF32- encoded and no conversions or
      checks are made to ensure this.
   */
   string::string(const char32_t* in)
   {
      data.clear();

      while(*in != 0x00000000)
      {
         data.push_back(*in);
         in++;
      }
   }

   //! Create from character array
   /*!
      Creates wcl string instance from data provided in character array.
      It is presumed that the character array data is encoded in either
      UTF-8 or ASCII format.
   */
   string::string(const char* in)
   {
      data.clear();

      std::string result;

      utf8::replace_invalid(in, in + strlen(in), back_inserter(result));
      utf8::utf8to32(result.begin(), result.end(), back_inserter(data));
   }

   //! Create from character array, with
   /*!
      Creates wcl string instance from data provided in character array.
      It is presumed that the character array data is encoded in either
      UTF-8 or ASCII format.
   */
   string::string(const char* in, size_t length)
   {
      data.clear();

      std::string result;

      utf8::replace_invalid(in, in + length, back_inserter(result));
      utf8::utf8to32(result.begin(), result.end(), back_inserter(data));
   }

   //! Copy construct from STL string
   /*!
      Creates wcl string instance from data provided in std::string.
      It is presumed that the std::string data is encoded in UTF-8 or
      ASCII format.
   */
   string::string(const std::string& in)
   {
      data.clear();

      std::string result;
      
      utf8::replace_invalid(in.begin(), in.end(), back_inserter(result));
      utf8::unchecked::utf8to32(result.begin(), result.end(), back_inserter(data));
   }

   //! Destructor brief
   string::~string()
   {
   }

   //! Get pointer to internal data
   /*!
      \return pointer to the string data.
      \warning The string data is not required to be null-terminated
   */
   const char32_t* string::getptr() const
   {
      return &data[0];
   }

   //! Generate C-style string
   /*
      The string's contents are converted from internal UTF-32 format to an UTF-8 encoding
      and returned as C-style string.

      \return <code>const char*</code> containing null-terminated string in UTF-8 format.

      \warning Preferred method to get C-style strings out is to instead call string.std_str().c_str(),
               which gets you actual const char pointer instead of const pointer to const char that
               points to static structures.
      \warning This is also EXTREMELY THREAD-UNSAFE.
   */
   /*
   const char* const string::c_str() const
   {
      static std::string result;

      result.resize(0);
      utf8::utf32to8(data.begin(), data.end(), std::back_inserter(result));
      return result.c_str();
   }
   */

   //! Generate std::string
   /*
      The string's contents are converted from internal UTF-32 format to an UTF-8 encoding
      and returned as C++ STL string.

      \return <code>const std::string</code> containing the string in UTF-8 format.
   */
   const std::string string::std_str() const
   {
      std::string result;
      utf8::utf32to8(data.begin(), data.end(), std::back_inserter(result));
      return result;
   }

   //! Generate c-style string
   /*
      The C-style string is written to array given as a parameter, no allocation is made
      so the array must be pre-allocated.

      \return nothing
   */
   void string::to_c_str(char* array) const
   {
      std::vector<char> utf8_result;

      utf8::utf32to8(data.begin(), data.end(), std::back_inserter(utf8_result));

      utf8_result.push_back('\0');
      memcpy(array, &utf8_result[0], utf8_result.size());
   }

   //! Return integer value
   /*
      \return Integer value represented in the string.
   */
   uint32_t string::to_uint32()
   {
      return strtoul(this->std_str().c_str(), nullptr, 0);
   }

   //! Return floating point value
   /*
      \return Double-precision floating point value represented in the string.
   */
   double string::to_float()
   {
      return atof(this->std_str().c_str());
   }

   //! Return the length of the string
   /*
      \return size_t length of the string
   */
   size_t string::length() const
   {
      return data.size();
   }

   //! Split a string by a delim
   /*!
      Returns a vector of string objects that are substrings cut at given deliminators.

      \param   delim A string of characters that are to be used as deliminators.

      \return  A vector of string objects computed by splitting the string around given deliminator.
   */
   std::vector<string> string::split(const string& delim)
   {
      std::vector<string> rval;
      if (data.size() == 0)
         return rval;

      size_t pos = 0;

      for (size_t i = 0; i < data.size(); ++i)
      {
         if (match_char32(data[i], delim))
         {
            while(match_char32(data[pos], delim))
               pos++;

            string temp = this->substr(pos, i - pos);

            if (temp.length() != 0)
            {
               rval.push_back(temp);
               pos = i;
            }
         }
      }

      while(match_char32(data[pos], delim))
         pos++;

      string temp = this->substr(pos, data.size() - pos);
      if (temp.length() != 0)
         rval.push_back(temp);

      return rval;
   }

   //! Generate substring
   /*!
      Returns a newly constructed string object with its value initialised to
      a copy of substring of this object.

      The substring is part of the object that starts at character position pos
      and spans len characters.

      \param   pos   Position of the first character to be copied as a substring
      \param   len   Number of characters to include in the substring

      \return A string object with a substring of this object.
   */
   string string::substr(size_t pos, size_t len) const
   {
      string rval;
      for (uint32_t it = pos; it < pos + len; ++it)
      {
         if (it > data.size())
            return rval;
         rval.data.push_back(data[it]);
      }
      return rval;
   }

   //! Does a string contain a sequence
   /*!
      Searches a string for a sequence specified by argument seq.

      \return <code>true</code> if string contains sequence.
   */
   bool string::contains(const string& seq)
   {
      if (this->length() < seq.length())
         return false;

      size_t lastpos = this->length() - seq.length() + 1;

      for (size_t i = 0; i < lastpos; ++i)
      {
         if (this->substr(i, seq.length()) == seq)
            return true;
      }

      return false;
   }

   //! Checks if the string is empty
   /*!
      \return <code>true</code> if the string is empty.
   */
   bool string::empty()
   {
      if (this->length() == 0)
         return true;

      return false;
   }

   //! Clears all data from the string
   /*!
   */
   void string::clear()
   {
      data.clear();
   }

   //! Copy assignment operator
   /*!
   */
   string& string::operator=(const string& other)
   {
      if (this == &other)
         return *this;

      data.resize(other.data.size());
      memcpy(&data[0], &other.data[0], other.data.size() * sizeof(char32_t));

      return *this;
   }

   //! Move assignment operator
   /*!
   */
   string& string::operator=(string&& other)
   {
      // Disallow moving to itself
      if (this != &other)
      {
         data = std::move(other.data);
      }

      return *this;
   }

   //! Concatenate (addition) operator
   /*!
      Allows concatenating strings together, may be used with
      types conversible to strings, such as integers and floats.

      \return Concatenated string

      Example usage:
      \code
         string s1 = U"First string";
         string s2 = U"Second string";

         int32_t i = 252;

         string s = s1 + s2 + " " + i + U" is a number";

         std::cout << s.std_str().c_str() << "\n";
      \endcode

      Results in
      \code
         First stringSecond String 252 is a number
      \endcode
   */
   string string::operator+(const string& other) const
   {
      string rval(*this);
      for (auto c : other.data)
         rval.data.push_back(c);

      return rval;
   }

   string& string::operator+=(const string& other)
   {
      *this = *this + other;
      return *this;
   }

   //! Comparison
   /*!
      Compares the values of two strings together
      \return true if two strings match.  Otherwise false.
   */
   bool string::operator==(const string& other) const
   {
      if (data.size() != other.data.size())
         return false;

      for (size_t i = 0; i < data.size(); ++i)
         if (data[i] != other.data[i])
            return false;

      return true;
   }
   //! Comparison, not equal
   /*!
   */
   bool string::operator!=(const string& other) const
   {
      return !(*this == other);
   }
   //! Comparison, before
   /*!
      Compares the two strings by their ASCII value.
   */
   bool string::operator<(const string& other) const
   {
      for (size_t i = 0; i < std::min(length(), other.length()); ++i)
         if (data[i] < other.data[i])
            return true;

      return false;
   }

   bool string::operator>(const string& other) const
   {
      for (size_t i = 0; i < std::min(length(), other.length()); ++i)
         if (data[i] > other.data[i])
            return true;

      return false;
   }


   //! Conversion to const char*
   /*!
      Converts string to const char*  (same as using c_str())

   string::operator const char*()
   {
      return this->c_str();
   }
   */
}
