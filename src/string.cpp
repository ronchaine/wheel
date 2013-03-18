/*!
   @file
   \brief Contains implementations for the string class and string manipulation functions.
   \author Jari Ronkainen
*/

#include "../include/wheel_core_string.h"

#include "utf8.h"

#include <utility>
#include <cstdlib>
#include <cstring>

#include <iostream>

#include <cassert>

namespace wheel
{
   namespace core
   {
      //! Default constructor.  Does nothing at the moment.
      string::string()
      {
      }

      //! Default copy constructor.
      string::string(const string& other)
      {
         data.resize(other.data.size());
         memcpy(&data[0], &other.data[0], other.data.size() * sizeof(char32_t));
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
         utf8::utf8to32(result.begin(), result.end(), back_inserter(data));
      }

      //! Destructor brief
      string::~string()
      {
      }

      //! Get pointer to internal data
      /*!
         Returns an pointer to char32_t-array of text data.
      */
      const char32_t* string::getptr() const
      {
         return &data[0];
      }

      //! Generate C-style string
      /*
         Returns data converted from internal UTF-32 format to C
         char array in UTF-8 encoding.
      */
      const char* string::c_str() const
      {
         std::string result;

         utf8::utf32to8(data.begin(), data.end(), std::back_inserter(result));
         return result.c_str();
      }

      //! Generate std::string
      /*
         Returns data converted from internal UTF-32 format to
         an UTF-8 encoded std::string.
      */
      const std::string string::std_str() const
      {
         std::string result;
         utf8::utf32to8(data.begin(), data.end(), std::back_inserter(result));
         return result;         
      }

      //! Return integer value
      /*
         Returns integer value represented in the string.
      */
      uint32_t string::to_uint32()
      {
         return strtoul(this->c_str(), nullptr, 0);
      }

      //! Return floating point value
      /*
         Returns double-precision floating point value represented in the string.
      */
      double string::to_float()
      {
         return atof(this->c_str());
      }

      //! Return the length of the string
      /*
         Returns size_t length of the string
      */
      size_t string::length() const
      {
         return data.size();
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
      string string::substr(size_t pos, size_t len)
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

         \return  true if string contains sequence.
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

      //! Copy assignment operator
      /*!
      */
      string& string::operator=(string& other)
      {
         swap(*this, other);
         /*
         if (this == &other)
            return *this;

         data.resize(other.data.size());
         memcpy(&data[0], &other.data[0], other.data.size() * sizeof(char32_t));
         */
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

            std::cout << s.c_str() << "\n";
         \endcode

         Results in
         \code
            First stringSecond String 252 is a number
         \endcode
      */
      string string::operator+(const string& other) const
      {
         string rval(getptr());
         for (char32_t c : other.data)
            rval.data.push_back(c);

         return rval;
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
}

/*!
   \related string
   \brief Allows wcl strings to work with ostreams (i.e. cout)
*/
std::ostream& operator<<(std::ostream& out, const wcl::string& str)
{
   out << str.std_str();
   return out;
}
