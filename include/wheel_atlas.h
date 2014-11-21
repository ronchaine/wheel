#ifndef WHEEL_ATLAS_HEADER
#define WHEEL_ATLAS_HEADER

#include <cstdint>
#include <list>

#include "wheel_core_common.h"
#include "wheel_core_debug.h"
#include "wheel_image_common.h"

namespace wheel
{
   /*!
      \brief Class for atlasing textures

      The Atlas class packs images in a single GPU texture.  A single Atlas
      can contain only images of the same format.
   */
   class Atlas
   {
      private:
      public:
         uint32_t    flags;
         int32_t     minsize;
         std::list<rect_t> unused, mapped;
         uint32_t    offset_x, offset_y, width, height;

         Atlas();
        ~Atlas();

         rect_t Fit(uint32_t w, uint32_t h);

         void SetFormat(uint32_t format);

         void Split(const rect_t& r);
         void Prune(const rect_t& r);
         void Reset();
   };
}

#endif