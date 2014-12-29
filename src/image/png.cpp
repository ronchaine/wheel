/*!
   @file
   \brief Contains implementations for PNG operations
   \author Jari Ronkainen
*/

// This is pretty much the same as the png_old.cpp, only this does not use the PNG class,
// as it was pretty damn stupid design choice anyways.

#include "../../include/wheel_core_debug.h"
#include "../../include/wheel_core_string.h"
#include "../../include/wheel_core_utility.h"
#include "../../include/wheel_core_library.h"
#include "../../include/wheel_image_png.h"
#include <cstring>

#include "../../include/3rdparty/miniz.c"

#define ZLIB_CHUNK 262144

namespace wheel
{
   namespace image
   {
      /*!
         Struct to store PNG chunk data
      */
      struct PNGChunk
      {
         uint32_t    len;
         char        type[4];
         uint8_t*    data;
         uint32_t    crc;

         PNGChunk()
         {
            data = nullptr;
         }
        ~PNGChunk()
         {
            if (data != nullptr)
               free(data);
         }
      };

      /*!
         Use zlib to deflate a buffer to a destination buffer
      */
      size_t z_compress(const void* src, size_t len, buffer_t& destination)
      {
         z_stream stream;

         uint8_t temp_buffer[ZLIB_CHUNK];

         buffer_t buffer;

         stream.zalloc = Z_NULL;
         stream.zfree = Z_NULL;
         stream.next_in = (uint8_t*)src;
         stream.avail_in = len;
         stream.next_out = temp_buffer;
         stream.avail_out = ZLIB_CHUNK;

         deflateInit(&stream, Z_BEST_COMPRESSION);

         while (stream.avail_in != 0)
         {
            if (deflate(&stream, Z_NO_FLUSH) != Z_OK);
            {
               log << "zlib error - deflating failed\n";
               return 0;
            }

            if (stream.avail_out == 0)
            {
               buffer.insert(buffer.end(), temp_buffer, temp_buffer + ZLIB_CHUNK);
               stream.next_out = temp_buffer;
               stream.avail_out = ZLIB_CHUNK;
            }
         }

         int deflate_result = Z_OK;
         while (deflate_result == Z_OK)
         {
            if (stream.avail_out == 0)
            {
               buffer.insert(buffer.end(), temp_buffer, temp_buffer + ZLIB_CHUNK);
               stream.next_out = temp_buffer;
               stream.avail_out = ZLIB_CHUNK;
            }

            deflate_result = deflate(&stream, Z_FINISH);
         }

         if (deflate_result != Z_STREAM_END)
         {
            log << "zlib error - deflating failed\n";
            return 0;
         }

         buffer.insert(buffer.end(), temp_buffer, temp_buffer + ZLIB_CHUNK - stream.avail_out);
         deflateEnd(&stream);

         destination.swap(buffer);

         return destination.size();
      }

      /*!
         Use zlib to inflate a buffer to a destination
      */
      size_t z_uncompress(const void* src, size_t len, buffer_t& destination)
      {
         z_stream stream;

         stream.zalloc = Z_NULL;
         stream.zfree = Z_NULL;
         stream.opaque = Z_NULL;

         stream.avail_in = stream.total_in = len;
         stream.next_in = (uint8_t*)src;

         stream.avail_out = ZLIB_CHUNK;

         uint8_t temp_buffer[ZLIB_CHUNK];
         buffer_t buffer;

         int ret;

         if ((ret = inflateInit(&stream)) != Z_OK)
         {
            log << "zlib error - inflating failed\n";
            return 0;
         }

         while (ret != Z_STREAM_END)
         {
            stream.next_out   = temp_buffer;
            stream.avail_out  = ZLIB_CHUNK;

            ret = inflate(&stream, Z_NO_FLUSH);

            buffer.insert(buffer.end(), temp_buffer, temp_buffer + ZLIB_CHUNK - stream.avail_out);
         }

         (void)inflateEnd(&stream);

         destination.swap(buffer);

         return destination.size();
      }

      /*!
         Use zlib to inflate a buffer to a destination
      */
      size_t z_uncompress(const buffer_t& source, buffer_t& destination)
      {
         if (source.size() == 0)
            return 0;

         log << "source:" << source.size() << "\n";

         return z_uncompress(&source[0], source.size()-1, destination);
      }

      //! Default PNG decoding function
      /*!

      */
      uint32_t decode_png(const wheel::string& name, buffer_t& buffer)
      {
         std::vector<PNGChunk> chunks;
 
         buffer.seek(8);

         uint32_t idat_count = 0;
/*
         while(buffer.pos() < buffer.size())
         {
            //buffer.seek(buffer.pos() + 1);
            if (buffer.size() < buffer.pos() + sizeof(uint32_t))
            {

            }
         }
*/
         Library::AddBuffer(WHEEL_RESOURCE_IMAGE, name, buffer);

         return WHEEL_OK;
      }
   }
}