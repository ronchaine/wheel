/*!
   @file
   \brief Contains definitions for the string class and string manipulation functions.
*/

#ifndef WHEEL_STRING_HEADER
#define WHEEL_STRING_HEADER

#include "wheel_core_common.h"

#include <cstdint>
#include <vector>
#include <string>
#include <locale>
#include <memory>

namespace wheel
{
  /*!
     \brief Unicode string class

     The string class stores series of char32_t characters in an
     array.  The class can be used in similar manner as the std::string class.

     Strings stored in the class are UTF-32 -encoded.  They are not guaranteed to be
     null-terminated, so it is not recommended to use raw pointers obtained with getptr().
  */
  class string
  {
     private:
        std::vector<char32_t>   data;

     public:
        // Constructors

        // Standard
        string();

        // Copy
        string(const string& other);

        // Move
        string(string&& other);

        // Convert
        string(const char*);
        string(const std::string&);
        string(const char32_t*);
        string(const char*, size_t length);

        string(double);

        string(int32_t);
        string(int64_t);
        string(uint32_t);

        // Destructor
        ~string();

        // Convenience (stupid) constructors
        string(const char32_t c) { data.push_back(c); }

        // C++ stl compatibility
        std::vector<char32_t>::iterator begin() { return data.begin(); }
        std::vector<char32_t>::const_iterator begin() const { return data.begin(); }
        std::vector<char32_t>::iterator end() { return data.end(); }
        std::vector<char32_t>::const_iterator end() const { return data.end(); }

        // Operator overloads
        string& operator=(string&& other);
        string& operator=(const string& other);
        string operator+(const string& other) const;
        string& operator+=(const string& other);

        bool operator==(const string& other) const;
        bool operator!=(const string& other) const;
        bool operator<(const string& other) const;
        bool operator>(const string& other) const;

        // Conversion operators
//            operator const char*();

        // Get pointer to the string
        const char32_t* getptr() const;

        // Implicit conversion to std::string
        operator std::string() const;

        // Get C string
//            const char* const c_str() const;
        void to_c_str(char* array) const;

        // Returns std string
        const std::string std_str() const;

        // Returns array of strings
        std::vector<string> split(const string& delim = " \n\t");

        // Return unsigned integer
        uint32_t to_uint32();

        // Return floating point value
        double to_float();

        // Return string size
        size_t length() const;

        // Substring
        string substr(size_t start, size_t length) const;

        // Contains
        bool contains(const string&);

        // Is empty?
        bool empty();

        // Make it empty
        void clear();

        // Friends
        /*!
           \brief Allows wcl strings to work with ostreams (i.e. cout)
        */
        friend inline std::ostream& operator<<(std::ostream& out, const wheel::string& str)
        {
           out << str.std_str();
           return out;
        }

        // FVN-1a
        inline size_t hash() const noexcept;
  };
}

//! wheel::Hash specialisation for wheel::core::string
namespace wheel
{
  // 64-bit version of string hash
  template<>
  class Hash<string, true>
  {
    public:
      size_t operator()(const string& s) const noexcept
      {
        size_t hash  = 0xCBF29CE484222325;
        const uint64_t prime = 0x100000001B3;

        uint8_t* bytep = (uint8_t*)s.getptr();

        for (size_t it = 0; it < s.length() * 4; ++it)
        {
          hash = (hash ^ *(bytep+it)) * prime;
        }

      return hash;
      }
 };

  // 32-bit version of string hash
  template<>
  class Hash<string, false>
  {
    public:
      size_t operator()(const string& s) const noexcept
      {
        size_t hash = 0x811C9DC5;
        const uint32_t prime = 0x1000193;

        uint8_t* bytep = (uint8_t*)s.getptr();

        for (size_t it = 0; it < s.length() * 4; ++it)
        {
          hash = (hash ^ *(bytep+it)) * prime;
        }

        return hash;
      }
  };

  /*!
    Return hash of the string, note that the hash is not currently cached.

    \return FNV-1a hash.
  */
  inline size_t string::hash() const noexcept
  {
    wheel::Hash<wheel::string> rval_hash;
    return rval_hash(*this);
  }

  /*!
    Specialisation for buffer_t to write strings
  */
  template<>
  inline void buffer_t::write(const string& input)
  {
    std::string actual = input.std_str();
    this->reserve(this->size() + actual.size());

    for (auto c : actual)
    {
      this->push_back(c);
    }
  }
}

namespace wheel {
  /*!
    compares a character to number of other characters

    \param string string of characters to compare to.
  */
  inline bool match_char32(char32_t c, const string& chrs)
  {
    for (char32_t c_c : chrs)
    {
      if (c == c_c)
        return true;
    }
    return false;
  }  
}

namespace std {
  //! std::hash specialisation for wheel::core::string
  template<>
    struct hash<wheel::string>
    {
      size_t operator()(const wheel::string& __s) const noexcept
      { 
        return __s.hash();
      }
    };
}

//std::ostream& operator<<(std::ostream& out, const wheel::string& str);
#ifndef WHEEL_NAMESPACE_CORE_ALIASES_DEFINED
#define WHEEL_NAMESPACE_CORE_ALIASES_DEFINED
namespace wcl = wheel;
#endif //WHEEL_NAMESPACE_ALIASES_DEFINED

#endif //WHEEL_STRING_HEADER
