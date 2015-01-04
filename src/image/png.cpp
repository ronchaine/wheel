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
#include "../../include/wheel_image_decoders.h"
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
         WCL_DEBUG << "=== Loading PNG (" << name << ")\n";

         std::vector<PNGChunk*> chunks;
 
         buffer.seek(8);

         uint32_t idat_count = 0;

         WCL_DEBUG_VERBOSE << "+ Reading chunks...\n";

         // Read chunks to buffer.
         while(buffer.pos() < buffer.size())
         {
            PNGChunk* next = new PNGChunk();

            // Read chunk length
            if (!buffer.can_read(sizeof(next->len)))
            {
               delete next;
               return WHEEL_UNEXPECTED_END_OF_FILE;
            }

            next->len = buffer.read_le<uint32_t>();

            // Read chunk type
            if (!buffer.can_read(sizeof(next->type)))
            {
               delete next;
               return WHEEL_UNEXPECTED_END_OF_FILE;
            }

            strncpy(next->type, (const char*)(&buffer[0] + buffer.pos()), 4);
            string s(next->type, 4);
            buffer.seek(buffer.pos() + 4);

            // Read chunk data
            if (!buffer.can_read(next->len))
            {
               delete next;
               return WHEEL_UNEXPECTED_END_OF_FILE;
            }

            next->data = (uint8_t*) malloc(next->len);
            memcpy(next->data, (&buffer[0] + buffer.pos()), next->len);
            buffer.seek(buffer.pos() + next->len);

            // Read chunk CRC checksum
            if (!buffer.can_read(sizeof(next->crc)))
            {
               delete next;
               return WHEEL_UNEXPECTED_END_OF_FILE;
            }

            next->crc = buffer.read_le<uint32_t>();

            // Check CRC
            uint32_t crc_check = 0xffffffff;
            crc_check = update_crc(crc_check, (uint8_t*)next->type, sizeof(uint32_t));
            crc_check = update_crc(crc_check, next->data, next->len);
            crc_check ^= 0xffffffff;

            if (next->crc == crc_check)
            {
               WCL_DEBUG_VERBOSE << "-- read chunk: " << s << "\n";
               chunks.push_back(next);
               if (s == "IEND")
               {
                  WCL_DEBUG_VERBOSE << "\n";
                  break;
               }
            } else {
               WCL_WARNING << "-- png chunk crc mismatch. (" << next->crc << "vs." << crc_check << ")\n";
            }
         }

         WCL_DEBUG_VERBOSE << "+ PNG reading complete, decoding...\n";

         Image* image = new Image;

         // Normally PNG should have IHDR as its first chunk (by specification), but we are lenient.
         size_t   it = 0;
         bool     found_ihdr = false;
         for (it = 0; it < chunks.size(); ++it)
         {
            string s(chunks[it]->type, 4);
            if (s == "IHDR")
            {
               found_ihdr = true;
               break;
            }
         }

         if (!found_ihdr)
         {
            WCL_ERROR << "-!- no IHDR chunk found, bailing out.\n";
            for (auto chunk : chunks)
               free(chunk->data);

            return WHEEL_INVALID_FORMAT;
         }

         WCL_DEBUG_VERBOSE << "-- parsing IHDR chunk\n";

         image->width      = *(uint32_t*)chunks[it]->data;
         image->height     = *(uint32_t*)(chunks[it]->data + 4);
         image->bpp        = *(uint8_t*)(chunks[it]->data + 8);

         uint8_t imgtype   = *(uint8_t*)(chunks[it]->data + 9);
         uint8_t cmethod   = *(uint8_t*)(chunks[it]->data + 10);
         uint8_t filter    = *(uint8_t*)(chunks[it]->data + 11);
         uint8_t interlace = *(uint8_t*)(chunks[it]->data + 12);

         if (!big_endian())
         {
            image->width  = endian_swap(image->width);
            image->height = endian_swap(image->height);
         }

         WCL_DEBUG_VERBOSE << "++ Image header data\n";
         WCL_DEBUG_VERBOSE << "---- size: " << image->width << "x" << image->height << "x" << (uint32_t)image->bpp << "bpp \n";

         WCL_DEBUG_VERBOSE << "---- type: ";

         bool supported_format = true;

         if (image->bpp < 8)
         {
            supported_format = false;
            WCL_DEBUG_VERBOSE << "-!-  bit depths below 8 are not currently supported.\n";
         }

         if (imgtype == 0)
         {
            image->channels = 1;
            WCL_DEBUG_VERBOSE << "Greyscale\n";
         } else if (imgtype == 2) {
            image->channels = 3;
            WCL_DEBUG_VERBOSE << "RGB\n";
         } else if (imgtype == 3) {
            image->channels = 1;
            WCL_DEBUG_VERBOSE << "Paletted\n";
         } else if (imgtype == 4) {
            image->channels = 2;
            WCL_DEBUG_VERBOSE << "Grayscale-alpha\n";
         } else if (imgtype == 6) {
            image->channels = 4;
            WCL_DEBUG_VERBOSE << "RGBA\n";
         } else {
            image->channels = 0;
            WCL_DEBUG_VERBOSE << "Unknown\n";
         }

         WCL_DEBUG_VERBOSE << "---- " << image->channels << " channel(s)\n";

         WCL_DEBUG_VERBOSE << "---- Compression method: ";
         if (cmethod == 0)
         {
            WCL_DEBUG_VERBOSE << "inflate / deflate\n";
         } else {
            supported_format = false;
            WCL_DEBUG_VERBOSE << "unknown\n";
         }

         WCL_DEBUG_VERBOSE << "---- Filtering: ";
         if (filter == 0)
         {
            WCL_DEBUG_VERBOSE << "adaptive\n";
         } else {
            supported_format = false;
            WCL_DEBUG_VERBOSE << "unknown\n";
         }

         WCL_DEBUG_VERBOSE << "---- Interlacing: ";
         if (interlace == 0)
         {
            WCL_DEBUG_VERBOSE << "no interlacing\n";
         } else if (interlace == 1) {
            supported_format = false;
            WCL_DEBUG_VERBOSE << "Adam7 (unsupported)\n";
         } else {
            supported_format = false;
            WCL_DEBUG_VERBOSE << "unknown\n";
         }

         WCL_DEBUG_VERBOSE << "\n";

         if (!supported_format)
         {
            WCL_ERROR << "Unsupported PNG format.\n";
            for (auto chunk : chunks)
               free(chunk->data);

            return WHEEL_INVALID_FORMAT;
         }

         // We don't bother with the palette entry on images that are not paletted
         if (imgtype == 3)
         {
            WCL_DEBUG_VERBOSE << "+ Parsing palette information...\n";

            uint32_t plte = ~0;
            uint32_t trns = ~0;

            for (it = 0; it < chunks.size(); ++it)
            {
               string s(chunks[it]->type, 4);
               if (s == "PLTE")
                  plte = it;
               if (s == "tRNS")              
                  trns = it;
            }

            if (plte == ~0)
            {
               WCL_ERROR << "-- Paletted image with no palette chunk?\n";
               for (auto chunk : chunks)
                  free(chunk->data);

               return WHEEL_INVALID_FORMAT;
            }

            if (chunks[plte]->len % 3 != 0)
            {
               WCL_ERROR << "-- Invalid PLTE chunk.\n";
               for (auto chunk : chunks)
                  free(chunk->data);

               return WHEEL_INVALID_FORMAT;
            }

            size_t entries = chunks[plte]->len / 3;
            size_t trns_entries = 0;

            if (trns == ~0)
            {
               WCL_DEBUG_VERBOSE << "-- no transparency\n";
            } else {
               trns_entries = chunks[trns]->len;
            }

            WCL_DEBUG_VERBOSE << "-- palette entries: " << entries << "\n";

            uint8_t r, g, b, a;

            for (size_t i = 0; i < entries; ++i)
            {
               r = chunks[plte]->data[3 * i];
               g = chunks[plte]->data[3 * i + 1];
               b = chunks[plte]->data[3 * i + 2];

               if (i < trns_entries)
                  a = chunks[trns]->data[i];
               else
                  a = 0xff;

               WCL_DEBUG_VERBOSE << "---- r:" << (uint32_t)r << " g:" << (uint32_t)g << " b:" << (uint32_t)b << " a:" << (uint32_t)a << "\n" ;
               image->palette.push_back((r << 24) + (g << 16) + (b << 8) + a);
            }
            WCL_DEBUG_VERBOSE << "\n";
         }


         WCL_DEBUG << "+ Concatenating IDAT chunks...\n";

         // Then IDAT stuff...
         buffer_t concatenated_data, image_data;
         for (auto c : chunks)
         {
            string s(c->type, 4);

            if (s == "IDAT")
            {
               WCL_DEBUG_VERBOSE << "-- Found IDAT chunk, size: " << c->len << "B\n";
               concatenated_data.insert(concatenated_data.end(), c->data, c->data + c->len);
            }
         }
         WCL_DEBUG_VERBOSE << "-- Total size: " << concatenated_data.size() << " bytes\n\n";

         WCL_DEBUG_VERBOSE << "+ Uncompressing...\n";
         z_uncompress((const void*)&concatenated_data[0], concatenated_data.size(), image_data);
         WCL_DEBUG_VERBOSE << "-- Uncompressed size: " << image_data.size() << " bytes\n\n";

         buffer_t* f_ptr = image->data_ptr();
         f_ptr->clear();

         image_data.seek(0);

         uint8_t scanline_filter;
         size_t col, decoded_bytes = 0;         

         WCL_DEBUG << "+ Decoding...\n";
         for (size_t row = 0; row < image->height; ++row)
         {
            scanline_filter = image_data.read<uint8_t>();

            col = 0;
            while(col < image->width)
            {
               if (image->bpp == 8)
               {
                  for (size_t ch = 0; ch < image->channels; ++ch)
                  {
                     uint8_t pd = image_data.read<uint8_t>();

                     // These make everything a bit cleaner
                     uint8_t a, b, c;

                     if (col == 0)
                        a = 0;
                     else
                        a = f_ptr->at(f_ptr->size() - 1 * image->channels);

                     if (row == 0)
                        b = 0;
                     else
                        b = f_ptr->at(f_ptr->size() - (image->channels * image->width));

                     if (a == 0 && b == 0)
                        c = 0;
                     else
                        c = f_ptr->at(f_ptr->size() - (row != 0) * (image->channels * image->width) - (col != 0) * 1 * image->channels);

                     if (scanline_filter == 0)        // No filter on this line
                        f_ptr->push_back(pd);
                     else if (scanline_filter == 1)   // Left
                     {
                        f_ptr->push_back(pd + a);
                     }
                     else if (scanline_filter == 2)   // Up
                     {
                        f_ptr->push_back(pd + b);
                     }
                     else if (scanline_filter == 3)   // Average
                     {
                        uint32_t result = (a + b) >> 1;
                        f_ptr->push_back(pd + result);
                     }
                     else if (scanline_filter == 4)   // Paeth
                     {
                        int32_t p  = a + b - c;
                        int32_t pa = abs(p - a);
                        int32_t pb = abs(p - b);
                        int32_t pc = abs(p - c);
                        uint8_t pr;
                        if ((pa <= pb) && (pa <= pc))
                           pr = a;
                        else if (pb <= pc)
                           pr = b;
                        else
                           pr = c;

                        f_ptr->push_back(pd + pr);
                     }
                     else // undefined
                     {
                        WCL_DEBUG << (int)scanline_filter << "\n";
                        assert(0 && "nonconforming PNG file");
                     }
                  }
                  decoded_bytes++;
                  col++;
               }
               else
               {
                  assert(0 && "nope!");
               }
            }
         }
         WCL_DEBUG_VERBOSE << "-- Decoded " << f_ptr->size() << " bytes of data.\n\n";

         // We're done with the chunks, let them go.
         for (auto chunk : chunks)
            free(chunk->data);

         WCL_DEBUG << "+ Loaded PNG, size:" << image->width << "x" << image->height << ", channels: " << image->channels << "\n";

         Library::AddResource(WHEEL_RESOURCE_IMAGE, name, image);

         return WHEEL_OK;
      }
   }
}