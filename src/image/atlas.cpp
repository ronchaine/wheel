/*!
   @file
   \brief Contains implementations for texture atlasing.
   \author Jari Ronkainen
*/

//#include "atlas.h"
#include "../include/wheel_atlas.h"

namespace wheel
{
   namespace image
   {
      bool rect_overlap(const rect_t& r1, const rect_t& r2)
      {
         int b1,b2,rg1,rg2;

         b1 = r1.y + r1.h;
         b2 = r2.y + r2.h;

         rg1 = r1.x + r1.w;
         rg2 = r2.x + r2.w;

         if (( b1 > r2.y ) && ( r1.y < b2 ) && ( rg1 > r2.x ) && ( rg2 > r1.x ))
            return true;

         return false;
      }
      bool rect_iscontained(const rect_t& r1, const rect_t& r2)
      {
         if ((r1.x >= r2.x) && (r1.x + r1.w <= r2.x + r2.w) && (r1.y >= r2.y) && (r1.y + r1.h <= r2.y + r2.h))
            return true;

         return false;
      }

      //! Default constructor
      Atlas::Atlas()
      {
         tex_id = 0;
         offset_x = 0;
         offset_y = 0;
         width = 0;
         height = 0;
         flags = 0;
         minsize = 4;

         internalformat = WHEEL_PIXEL_FMT_NONE;
      }

      //! Changes 
      void Atlas::SetFormat(uint32_t format)
      {
         internalformat = format;
      }

      //! Splits the atlas to smaller parts
      /*
         Splits the atlas by removing the rectangle given from free space.

         \param   r   a rectangle to use as a splitting surface.
      */
      void Atlas::Split( const rect_t& r )
      {
         rect_t temp;

         for( auto r2 : unused )
         {
            if (rect_overlap( r, r2 ))
            {
               if (r.x < r2.x + r2.w && r.x + r.w > r2.x)
               {
                  if (r.y > r2.y && r.y < r2.y + r2.h)
                  {
                     temp = r2;
                     temp.h = r.y - r2.y;
                     if (temp.h > minsize)
                        unused.push_front(temp);
                  }
                  if (r.y + r.h < r2.y + r2.h)
                  {
                     temp = r2;
                     temp.y = r.y + r.h;
                     temp.h = r2.y + r2.h - (r.y + r.h);
                     if (temp.h > minsize)
                        unused.push_front(temp);
                  }
               }
               if (r.y < r2.y + r2.h && r.y + r.h > r2.y)
               {
                  if (r.x > r2.x && r.x < r2.x + r2.w)
                  {
                     temp = r2;
                     temp.w = r.x - r2.x;
                     if (temp.w > minsize)
                        unused.push_front(temp);
                  }
                  if (r.x + r.w < r2.x + r2.w)
                  {
                     temp = r2;
                     temp.x = r.x + r.w;
                     temp.w = r2.x + r2.w - (r.x + r.w);
                     if (temp.w > minsize)
                        unused.push_front(temp);
                  }
               }
            }
         }
      }

      //! Fit an image into an atlas
      /*
         Given image width and height, returns a rectangle that resembles best fit to the
         current texture atlas for an image of the given size.  The best fit is determinated
         by MAXRECTS algorithm, as given in http://clb.demon.fi/files/RectangleBinPack.pdf

         \param   w   Width of the image to be added to the atlas
         \param   h   Height of the image to be added to the atlas

         \return <code>rect_t</code> containing texture coordinates for the image size given.
      */
      rect_t Atlas::Fit( uint32_t w, uint32_t h )
      {
         rect_t rval;
         rval.w = 0;
         rval.h = 0;

         unused.sort();

         for (auto it : unused)
         {
            if (( (int32_t)w < it.w ) && ( (int32_t)h < it.h ))
            {
               rval.x = it.x;
               rval.y = it.y;
               rval.w = w;
               rval.h = h;

               Split(rval);

               return rval;
            }
         }
         return rval;
      }

      //! Clears the atlas
      /*!
      */
      void Atlas::Reset()
      {
         unused.clear();
         rect_t r;
         r.x = 0;
         r.y = 0;
         r.w = width;
         r.h = height;
         usrflags = 0;

         unused.push_back(r);

         #warning TODO: Unimplemented feature -- needs to create a texture.
/*
         glBindTexture( GL_TEXTURE_2D, texid );

         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

         glPixelStorei(GL_PACK_ALIGNMENT, 1);
         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

         glTexImage2D( GL_TEXTURE_2D, 0, internalformat, tex_w, tex_h, 0, internalformat, GL_UNSIGNED_BYTE, 0 );
*/
      }

      //! Removes overlapping and useless rectangles.
      /*!
         \param   r   Rectangle to remove overlaps with.
      */
      void Atlas::Prune( const rect_t& r )
      {
         for (auto it = unused.begin(); it!=unused.end(); it++ )
         {
            if (rect_overlap(r, *it))
            {
               it = unused.erase(it);
               it--;
            }
         }
         for (auto i = unused.begin(); i!=unused.end(); ++i )
            for (auto j = i; j != unused.end(); ++j)
            {
               if ( i == j ) continue;
               if (rect_iscontained(*i, *j))
               {
                  i = unused.erase(i);
                  i--;
                  break;
               }
               if (rect_iscontained(*j, *i))
               {
                  j = unused.erase(j);
                  j--;
               }
            }
      }

      //! Basic destructor
      Atlas::~Atlas()
      {
         #warning FIXME: Textures need to be destroyed here
      }

   }
}


