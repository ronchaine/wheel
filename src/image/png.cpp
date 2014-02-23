/*!
   @file
   \brief Contains implementations for PNG operations
   \author Jari Ronkainen
*/

#include "../../include/wheel_core_debug.h"
#include "../../include/wheel_core_string.h"
#include "../../include/wheel_core_utility.h"

#include "../../include/wheel_image_png.h"

#include <cstring>

#warning Currently still using zlib for inflate/deflate, will switch to miniz later on.
#include <zlib.h> // TODO: Switch to miniz.

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

      /*!
         Internal command to read a uint32_t value independent of endianness
      */
      uint32_t read_nod_uint32(const buffer_t& buffer, size_t& bufptr)
      {
         uint32_t rval = *(uint32_t*)(&buffer[0] + bufptr);

         bufptr += sizeof(uint32_t);

         if (big_endian())
            return rval;

         rval = (rval & 0xff000000) >> 24
              | (rval & 0x00ff0000) >> 8
              | (rval & 0x0000ff00) << 8
              | (rval & 0x000000ff) << 24;

         return rval;
      }

      /*!
         Simple destructor
      */
      PNG::~PNG()
      {
         for (size_t i = 0; i < chunks.size(); ++i)
            delete chunks[i];
      }

      /*!
         Function that decodes idat data and writes it to default image buffer so it can be used as per any image.
      */
      void PNG::decode_idat_data()
      {
         buffer_t concatenated_data;

         PNGChunk* header = chunks[0];

         string s(header->type, 4);
         if (s != "IHDR")
         {
            log << "Failed decoding -- IHDR chunk\n";
            return;
         }

         uint32_t w, h, bpp, ct;

         if (big_endian())
         {
            w = *(uint32_t*)(header->data + 0);
            h = *(uint32_t*)(header->data + 4);
         } else {
            w  =  (*(uint32_t*)(header->data + 0)& 0xff000000) >> 24
               |  (*(uint32_t*)(header->data + 0)& 0x00ff0000) >> 8
               |  (*(uint32_t*)(header->data + 0)& 0x0000ff00) << 8
               |  (*(uint32_t*)(header->data + 0)& 0x000000ff) << 24;
            h  =  (*(uint32_t*)(header->data + 4)& 0xff000000) >> 24
               |  (*(uint32_t*)(header->data + 4)& 0x00ff0000) >> 8
               |  (*(uint32_t*)(header->data + 4)& 0x0000ff00) << 8
               |  (*(uint32_t*)(header->data + 4)& 0x000000ff) << 24;
         }

//         bpp = (uint32_t)(*(uint8_t*)(header->data + 8) * 4);
         bpp = (uint32_t)(*(uint8_t*)(header->data + 8));
         ct = *(uint32_t*)(header->data + 9);

         log << "Image metrics: " << w << "x" << h << "x" << bpp << "\n";

         for (auto c : chunks)
         {
            string s(c->type, 4);

            if (s == "IDAT")
               concatenated_data.insert(concatenated_data.end(), c->data, c->data + c->len);
         }

         buffer_t image_data;
         buffer_t decoded_data;

         if (concatenated_data.size() == 0)
            return;

         z_uncompress((const void*)&concatenated_data[0], concatenated_data.size(), image_data);

         log << "Image size: " << image_data.size() << "\n";

         size_t img_ptr = 0;

         uint32_t filter_type, col = 0;

         if (!(
               ((ct == 0) && ((bpp == 1) || (bpp == 2) || (bpp == 4) || (bpp == 8) || (bpp == 16)))
            || ((ct == 2) && ((bpp == 8) || (bpp == 16)))
            || ((ct == 3) && ((bpp == 8) || (bpp == 4) || (bpp == 2) || (bpp == 1)))
            || ((ct == 4) && ((bpp == 8) || (bpp == 16)))
            || ((ct == 6) && ((bpp == 8) || (bpp == 16)))
            ))
         {
            log << "Disallowed bit depth for colour type -- ct:" << ct << " bpp:" << bpp << "\n";
            return;
         }

         uint32_t sampledepth = 8;

         if (ct != 3)
            sampledepth = bpp;

/*
         for (uint32_t row = 0; row < h; row++)
         {
            filter_type = image_data[img_ptr];

            log << "filter: " << (uint32_t)image_data[img_ptr] << "\n";

            img_ptr++; col = 0;

            while(col < w)
            {
               // FIXME: Other than 8 bit values might be in reverse order

               if (sampledepth == 2)
               {
                  uint8_t values = image_data[img_ptr];

                  decoded_data.push_back(values & 0b00000011);
                  decoded_data.push_back((values & 0b00001100) >> 2);
                  decoded_data.push_back((values & 0b00110000) >> 4);
                  decoded_data.push_back((values & 0b11000000) >> 6);
               }
               else if (sampledepth == 4)
               {
                  uint8_t values = image_data[img_ptr];

                  decoded_data.push_back(values & 0b00001111);
                  decoded_data.push_back((values & 0b11110000) >> 4);
               }
               else if (sampledepth == 8)
               {
                  decoded_data.push_back(image_data[img_ptr]);
               }
               else if (sampledepth == 16)
               {
                  decoded_data.push_back(image_data[img_ptr]);
                  img_ptr++;
                  decoded_data.push_back(image_data[img_ptr]);
               }
               col++;
            }
         }
*/
      }

      /*!
         Displays image about the chunks in the PNG.
      */
      void PNG::DisplayInfo()
      {
         log << "PNG data\n";

         for (auto chunk : chunks)
         {
            string s(chunk->type, 4);
            log << "   PNG Chunk: " << s << ", length " << chunk->len << "\t\tCRC:" << chunk->crc << "\n";

            if (s == "IHDR")
            {
               log << "      Header data\n";
               log << "         Width: " << GetWidth() << "   Height: " << GetHeight() << "\n";

               uint32_t bpp = (uint32_t)(*(uint8_t*)(chunk->data + 8) * 4);
               log << "         Bit depth: " << bpp << "\n";

               uint8_t val = *(uint8_t*)(chunk->data + 9);
               if (val == 0)
                  log << "         Greyscale image\n";
               else if (val == 2)
                  log << "         RGB image\n";
               else if (val == 3)
                  log << "         Paletted image\n";
               else if (val == 4)
                  log << "         Grayscale-alpha\n";
               else if (val == 6)
                  log << "         RGBA image\n";
               else
                  log << "         Unknown image format\n";

               val = *(uint8_t*)(chunk->data + 10);
               if (val == 0)
                  log << "         Compression method: Inflate/Deflate\n";
               else
                  log << "         Unknown/Unsupported compression method\n";

               log << "         Filter type " << (uint32_t)*(uint8_t*)(chunk->data + 11) << "\n";

               val = *(uint8_t*)(chunk->data + 11);
               if (val == 0)
                  log << "         No interlacing\n";
               else if (val == 1)
                  log << "         Adam7 interlacing\n";
               else
                  log << "         Unknown/Unsupported interlacing method\n";
            } else if (s == "PLTE") {
               log << "         Palette contents:\n";

               uint32_t entries = chunk->len / 3;
               for (uint32_t i = 0; i < entries; ++i)
               {
                  log << "          entry " << i << " \tr: " << (uint32_t)*(uint8_t*)(chunk->data + i * 3)
                      << "\tg: " << (uint32_t)*(uint8_t*)(chunk->data + i * 3 + 1)
                      << "\tb: " << (uint32_t)*(uint8_t*)(chunk->data + i * 3 + 2) << "\n";
               }
            } else if (s == "IDAT")
            {
               buffer_t inflated;
               log << "         Data chunk:\n";

               log << "          size: " << chunk->len << ", inflated:" << z_uncompress(chunk->data, chunk->len, inflated) << "\n";
            }
         }
      }

      /*!
         Returns width of the PNG image
      */
      uint32_t PNG::GetWidth()
      {
         PNGChunk* chunk = chunks[0];

         string s(chunk->type, 4);
         if (s == "IHDR")
         {
            if (big_endian())
               return *(uint32_t*)chunk->data;
            else
            {
               return
                  (*(uint32_t*)chunk->data & 0xff000000) >> 24
               |  (*(uint32_t*)chunk->data & 0x00ff0000) >> 8
               |  (*(uint32_t*)chunk->data & 0x0000ff00) << 8
               |  (*(uint32_t*)chunk->data & 0x000000ff) << 24;
            }
         }

         return 0;
      }

      /*!
         Returns height of the PNG image
      */
      uint32_t PNG::GetHeight()
      {
         PNGChunk* chunk = chunks[0];

         string s(chunk->type, 4);
         if (s == "IHDR")
         {
            if (big_endian())
               return *(uint32_t*)(chunk->data + 4);
            else
            {
               return
                  (*(uint32_t*)(chunk->data + 4)& 0xff000000) >> 24
               |  (*(uint32_t*)(chunk->data + 4)& 0x00ff0000) >> 8
               |  (*(uint32_t*)(chunk->data + 4)& 0x0000ff00) << 8
               |  (*(uint32_t*)(chunk->data + 4)& 0x000000ff) << 24;
            }
         }

         return 0;
      }

      /*!
         Reads a PNG image from memory buffer
      */
      uint32_t PNG::Load(const buffer_t& buffer)
      {
         size_t bufptr = 8;

         uint32_t idat_count = 0;

         while(bufptr < buffer.size())
         {
            PNGChunk* chunk = new PNGChunk;

            if (buffer.size() < bufptr + sizeof(uint32_t))
               return WHEEL_UNEXPECTED_END_OF_FILE;

            chunk->len = read_nod_uint32(buffer, bufptr);
            log << "length: " << chunk->len << "\n";

            if (buffer.size() < bufptr + sizeof(char) * 4)
               return WHEEL_UNEXPECTED_END_OF_FILE;
            
            strncpy(chunk->type, (const char*)(&buffer[0] + bufptr), 4);
            string s(chunk->type, 4);
            log << "type: " << s << "\n";
            bufptr += sizeof(char) * 4;

            if (buffer.size() < bufptr + chunk->len)
               return WHEEL_UNEXPECTED_END_OF_FILE;

            chunk->data = (uint8_t*)malloc(chunk->len);
            memcpy(chunk->data, (&buffer[0] + bufptr), chunk->len);
            bufptr += chunk->len;

            chunk->crc = read_nod_uint32(buffer, bufptr);

            uint32_t crc_check = 0xffffffff;

            crc_check = update_crc(crc_check, (uint8_t*)chunk->type, sizeof(uint32_t));
            crc_check = update_crc(crc_check, chunk->data, chunk->len);
            crc_check ^= 0xffffffff;

            if (chunk->crc == crc_check)
            {
               log << "crc OK\n";
               chunks.push_back(chunk);
            } else {
               log << "crc mismatch, skipping...\n";
            }

            if (s == "IEND")
            {
               format = WHEEL_IMAGEFORMAT_PNG;

               log << "Decoding IDAT chunks...\n";
               decode_idat_data();

               return WHEEL_OK;
            }
         }
         return WHEEL_UNEXPECTED_END_OF_FILE;
      }
   }
}