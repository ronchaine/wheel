/*!
   @file
   \brief General video/input interface
   \author Jari Ronkainen
*/

#ifndef WHEEL_VIDEO_H
#define WHEEL_VIDEO_H

#include "wheel_core_common.h"
#include "wheel_core_string.h"

#include <unordered_map>

namespace wheel
{
   class Shader
   {
      private:
      public:
         uint32_t             SetVertexSource(const wcl::string& vs);
         uint32_t             SetFragmentSource(const wcl::string& fs);

         virtual uint32_t     Compile();
         virtual uint32_t     Use();

         virtual ~Shader()    {}
   };

   struct vertex_t
   {
      float    x0, y0, z0;    // position
      uint16_t s0, t0;        // diffuse     (normalised, from 0 to 0xffff)
      uint16_t s1, t1;        // normal      (normalised, from 0 to 0xffff)
      uint8_t  r, g, b, a;    // colour
   /*
      uint16_t s3, t3;        // padding 1
      uint16_t s4, t4;        // more padding
   */
      vertex_t() {}
      vertex_t(float x, float y, float z) : x0(x), y0(y), z0(z) {}
      vertex_t(float x, float y, float z, uint16_t s, uint16_t t) : x0(x), y0(y), z0(z), s0(s), t0(t) {}
   };
}

#endif