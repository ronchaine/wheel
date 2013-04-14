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

     Strings stored in the class are UTF-32 -encoded.  They are not null-terminated, so it
     is not recommended to use raw pointers obtained with getptr().
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

        string(double);
        string(int64_t);

        // Destuctor
        ~string();

        // Operator overloads
        string& operator=(string&& other);
        string& operator=(const string& other);
        string operator+(const string& other) const;
        bool operator==(const string& other) const;
        bool operator!=(const string& other) const;
        bool operator<(const string& other) const;
        bool operator>(const string& other) const;

        // Conversion operators
//            operator const char*();

        // Get pointer to the string
        const char32_t* getptr() const;

        // Return C string
//            const char* const c_str() const;

        // Returns std string
        const std::string std_str() const;

        // Return unsigned integer
        uint32_t to_uint32();

        // Return floating point value
        double to_float();

        // Return string size
        size_t length() const;

        // Substring
        string substr(size_t start, size_t length);

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
  };
}

namespace std {
  //! std::hash specialisation for wheel::core::string
  template<>
    struct hash<wheel::string>
    : public __hash_base<size_t, wheel::string>
    {
      size_t
      operator()(const wheel::string& __s) const noexcept
      { return std::_Hash_impl::hash(__s.getptr(),
                                     __s.length() * sizeof(char32_t)); }
    };
}

//std::ostream& operator<<(std::ostream& out, const wheel::string& str);

#ifndef WHEEL_NAMESPACE_CORE_ALIASES_DEFINED
#define WHEEL_NAMESPACE_CORE_ALIASES_DEFINED
namespace wcl = wheel;
#endif //WHEEL_NAMESPACE_ALIASES_DEFINED
#endif //WHEEL_STRING_HEADER
