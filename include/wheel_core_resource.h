/*!
   @file
   \brief Contains resource loading definitions, needs PHYSFS
   \author Jari Ronkainen
*/

#ifndef WHEEL_RESOURCE_HEADER
#define WHEEL_RESOURCE_HEADER

#include "wheel_core_common.h"
#include "wheel_core_string.h"

#include <unordered_map>
#include <vector>

namespace wheel
{
   const char*       AppPath();
   const char*       UserPath();

   uint32_t          CreateDirectory(const char* dir);

   uint32_t          SetWritePath(const char* newpath);

   uint32_t          Filesystem_Init(int argc, char* argv[]);
   void              Filesystem_Deinit();

   uint32_t          AddToPath(const string& resource, const string& where);

   const buffer_t*   GetBuffer(const string& filename);
   size_t            BufferSize(const string& filename);

   bool              IsCached(const string& filename);

   uint32_t          Buffer(const string& filename);
   void              DeleteBuffer(const string& filename);

   void              EmptyCache();

//   template 
/*
   uint32_t          buffer_read_xint32(const buffer_t buffer, size_t& location);
   uint32_t          buffer_read_xint32_le(const buffer_t buffer, size_t& location);
*/
}

#endif