/*
  romi-rover

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  romi-rover is collection of applications for the Romi Rover.

  romi-rover is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.

 */

#include <stdexcept>
#include <jpeglib.h>
#include <png.h>
#include <setjmp.h>
#include <r.h>
#include <FileUtils.h>
#include "cv/ImageIO.h"

namespace romi {

    // 4mb for jpeg file should be fine and will limit reallocates.
        constexpr int JPEG_MEM_DST_MGR_BUFFER_SIZE  = (4096*1024);
        class jpeg_destination_mem_mgr
        {
        public:
                explicit jpeg_destination_mem_mgr(std::vector<uint8_t>& data)
                        : mgr(), data(data) {
                }
                jpeg_destination_mgr mgr;
                std::vector<uint8_t>& data;
        };

        static void jpeg_buffer_init( j_compress_ptr cinfo )
        {
                auto* dst = (jpeg_destination_mem_mgr*)cinfo->dest;
                dst->data.resize( JPEG_MEM_DST_MGR_BUFFER_SIZE, 0 );
                cinfo->dest->next_output_byte = dst->data.data();
                cinfo->dest->free_in_buffer = dst->data.size();
        }

        static void jpeg_buffer_terminate( j_compress_ptr cinfo )
        {
                auto* dst = (jpeg_destination_mem_mgr*)cinfo->dest;
                dst->data.resize( dst->data.size() - cinfo->dest->free_in_buffer );
        }

        static boolean jpeg_output_buffer_empty( j_compress_ptr cinfo )
        {
                auto* dst = (jpeg_destination_mem_mgr*)cinfo->dest;
                size_t oldsize = dst->data.size();
                dst->data.resize( oldsize + JPEG_MEM_DST_MGR_BUFFER_SIZE );
                cinfo->dest->next_output_byte = dst->data.data() + oldsize;
                cinfo->dest->free_in_buffer = JPEG_MEM_DST_MGR_BUFFER_SIZE;
                return true;
        }

        static void jpeg_mem_dest( j_compress_ptr cinfo, jpeg_destination_mem_mgr * dst )
        {
                cinfo->dest = (jpeg_destination_mgr*)dst;
                cinfo->dest->init_destination = jpeg_buffer_init;
                cinfo->dest->term_destination = jpeg_buffer_terminate;
                cinfo->dest->empty_output_buffer = jpeg_output_buffer_empty;
        }

        bool ImageIO::store_jpg(Image& image, bytevector& out)
        {
                std::vector<uint8_t> buffer(image.channels() * image.width());
                JSAMPLE* pBuffer = &buffer[0];
                struct jpeg_compress_struct cinfo{};
                struct jpeg_error_mgr jerr{};
                size_t index = 0;

                cinfo.err = jpeg_std_error(&jerr);
                jpeg_create_compress(&cinfo);
                jpeg_destination_mem_mgr dst_mem(out);
                jpeg_mem_dest( &cinfo, &dst_mem);

                cinfo.image_width = static_cast<JDIMENSION>(image.width());
                cinfo.image_height = static_cast<JDIMENSION>(image.height());
                if (image.type() == Image::BW) {
                        cinfo.input_components = 1;
                        cinfo.in_color_space = JCS_GRAYSCALE;
                } else {
                        cinfo.input_components = 3;
                        cinfo.in_color_space = JCS_RGB;
                }

                jpeg_set_defaults(&cinfo);
                jpeg_set_quality(&cinfo, 90, TRUE);
                jpeg_start_compress(&cinfo, TRUE);

                auto& data = image.data();

                while (cinfo.next_scanline < cinfo.image_height) {
                        if (image.type() == Image::BW) {
                                for (size_t i = 0; i < image.width(); i++)
                                        buffer[i] = (uint8_t) (data[index++] * 255.0f);
                        } else {
                                for (size_t i = 0, j = 0; i < image.width(); i++) {
                                        buffer[j++] = (uint8_t) (data[index++] * 255.0f);
                                        buffer[j++] = (uint8_t) (data[index++] * 255.0f);
                                        buffer[j++] = (uint8_t) (data[index++] * 255.0f);
                                }
                        }
                        jpeg_write_scanlines(&cinfo, &pBuffer, 1);
                }

                jpeg_finish_compress(&cinfo);
                jpeg_destroy_compress(&cinfo);
                return true;
        }

        bool ImageIO::store_jpg(Image& image, const char *filename)
        {
                bool retval = true;
                std::vector<uint8_t> outbuffer;
                store_jpg(image, outbuffer);
                try {
                        FileUtils::TryWriteVectorAsFile(filename, outbuffer);
                }
                catch (const std::exception &ex) {
                        retval = false;
                }
                return retval;
        }
        
        bool ImageIO::store_png(Image& image, const char *filename)
        {
                png_structp png_ptr = nullptr;
                png_infop info_ptr = nullptr;
                size_t x, y, k;
                png_bytepp row_pointers;

                FILE *fp = fopen(filename, "wb");
                if (fp == nullptr) {
                        r_err("ImageIO::store_png: Failed to open the file '%s'", filename);
                        return false;
                }

                png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr,
                                                  nullptr, nullptr);
                if (png_ptr == nullptr) {
                        r_err("ImageIO::store_png: png_create_write_struct failed");
                        return false;
                }
                
                info_ptr = png_create_info_struct(png_ptr);
                if (info_ptr == nullptr) {
                        png_destroy_write_struct(&png_ptr, nullptr);
                        r_err("ImageIO::store_png: png_create_info_struct failed");
                        return false;
                }
                
                if (setjmp(png_jmpbuf(png_ptr))) {
                        png_destroy_write_struct(&png_ptr, &info_ptr);
                        r_err("ImageIO::store_png: setjmp returned from error");
                        return false;
                }
                
                png_set_IHDR(png_ptr, info_ptr,
                             static_cast<png_uint_32>(image.width()),
                             static_cast<png_uint_32>(image.height()),
                             8, 
                             (image.type() == Image::BW)? PNG_COLOR_TYPE_GRAY : PNG_COLOR_TYPE_RGB,
                             PNG_INTERLACE_NONE,
                             PNG_COMPRESSION_TYPE_DEFAULT,
                             PNG_FILTER_TYPE_DEFAULT);
                
                row_pointers = (png_bytepp) png_malloc(png_ptr, image.height()
                                                       * sizeof(png_bytep));
                
                for (y = 0; y < image.height(); y++) {
                        row_pointers[y] = (png_bytep) png_malloc(png_ptr,
                                                                 image.width()
                                                                 * image.channels());
                }

                auto& data = image.data();
                
                if (image.type() == Image::BW) {
                        for (y = 0, k = 0; y < image.height(); y++) {
                                png_bytep row = row_pointers[y];
                                for (x = 0; x < image.width(); x++) {
                                        *row++ = (png_byte) (data[k++] * 255.0f);
                                } 
                        }
                        
                } else {
                        for (y = 0, k = 0; y < image.height(); y++) {
                                png_bytep row = row_pointers[y];
                                for (x = 0; x < image.width(); x++) {
                                        *row++ = (png_byte) (data[k++] * 255.0f);
                                        *row++ = (png_byte) (data[k++] * 255.0f);
                                        *row++ = (png_byte) (data[k++] * 255.0f);
                                }
                        }
                }
                
                png_init_io(png_ptr, fp);
                png_set_rows(png_ptr, info_ptr, row_pointers);
                png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
                
                for (y = 0; y < image.height(); y++)
                        png_free(png_ptr, row_pointers[y]);
                
                png_free(png_ptr, row_pointers);
                png_destroy_write_struct(&png_ptr, &info_ptr);
                
                fclose(fp);
                
                return true;
        }

        bool ImageIO::load(Image& image, const char *filename)
        {
                bool success = false;
                
                if (is_jpg(filename)) {
                        success = load_jpg(image, filename);
                        
                } else if (is_png(filename)) {
                        success = load_png(image, filename);
                        
                } else {
                        r_warn("Unsupported image type: '%s'", filename);
                }
                
                return success;
        }
        
        bool ImageIO::is_png(const char *filename)
        {
                FILE *fp;
                unsigned char buf[4];

                if ((fp = fopen(filename, "rb")) == nullptr)
                        return 0;

                if (fread(buf, 1, 4, fp) != 4) {
                        fclose(fp);
                        return 0;
                }
        
                fclose(fp);
                return (png_sig_cmp(buf, 0, 4) == 0)? 1 : 0;
        }
        
        bool ImageIO::is_jpg(const char *filename)
        {
                FILE *fp;
                unsigned char buf[3];

                if ((fp = fopen(filename, "rb")) == nullptr)
                        return 0;
                ;
                if (fread(buf, 1, 3, fp) != 3) {
                        fclose(fp);
                        return 0;
                }
                
                fclose(fp);
                
                return buf[0] == 0xff && buf[1] == 0xd8 && buf[2] == 0xff;
        }

        
        static jmp_buf setjmp_buffer;

        static void exit_error(j_common_ptr cinfo __attribute__((unused)))
        {
                longjmp(setjmp_buffer, 1);
        }



        bool ImageIO::load_jpg(Image& image, const char *filename)
        {
                bool retval = true;
                std::vector<uint8_t> image_file_data;
                std::vector<float> image_file_data_f;
                try {
                        FileUtils::TryReadFileAsVector(filename, image_file_data);
                        retval = load_jpg(image, image_file_data.data(), image_file_data.size());
                }
                catch (const std::exception &ex) {
                        retval = false;
                }

                return retval;
        }

        bool ImageIO::load_jpg(Image& image, const uint8_t *data, size_t len)
        {
                struct jpeg_error_mgr pub;
                struct jpeg_decompress_struct cinfo;
                JSAMPARRAY buffer;
                size_t row_stride;

                cinfo.err = jpeg_std_error(&pub);
                pub.error_exit = exit_error;

                if (setjmp(setjmp_buffer)) {
                        jpeg_destroy_decompress(&cinfo);
                        r_err("Failed to load the data. Not a JPEG?");
                        return false;
                }

                jpeg_create_decompress(&cinfo);
                jpeg_mem_src(&cinfo, data, len);
                jpeg_read_header(&cinfo, TRUE);
                jpeg_start_decompress(&cinfo);

                if (cinfo.output_components == 1
                    && cinfo.jpeg_color_space == JCS_GRAYSCALE) {
                        r_debug("8-bit grayscale JPEG");
                        image.init(Image::BW, cinfo.output_width, cinfo.output_height);
                } else if (cinfo.output_components == 3
                           && cinfo.out_color_space == JCS_RGB) {
                        r_debug("24-bit RGB JPEG");
                        image.init(Image::RGB, cinfo.output_width, cinfo.output_height);
                } else {
                        r_err("load_jpg: Unhandled JPEG format");
                        return false;
                }

                row_stride = cinfo.output_width * (size_t)cinfo.output_components;
                buffer = (*cinfo.mem->alloc_sarray)
                        ((j_common_ptr) &cinfo, JPOOL_IMAGE, static_cast<JDIMENSION>(row_stride), 1);


                while (cinfo.output_scanline < cinfo.output_height) {
                        jpeg_read_scanlines(&cinfo, buffer, 1);

                        unsigned int offset;
                        unsigned char* p = buffer[0];
                        auto& img = image.data();

                        if (image.type() == Image::BW) {
                                offset = (cinfo.output_scanline - 1) * cinfo.output_width;
                                for (size_t i = 0; i < cinfo.output_width; i++, offset++)
                                        img[offset] = (float) p[i] / 255.0f;

                        } else {
                                offset = 3 * (cinfo.output_scanline - 1) * cinfo.output_width;
                                for (size_t i = 0; i < cinfo.output_width; i++) {
                                        img[offset++] = (float) *p++ / 255.0f;
                                        img[offset++] = (float) *p++ / 255.0f;
                                        img[offset++] = (float) *p++ / 255.0f;
                                }
                        }
                }

                jpeg_finish_decompress(&cinfo);
                jpeg_destroy_decompress(&cinfo);

                return true;
        }

        bool ImageIO::load_png(Image& image, const char *filename)
        {
                size_t x, y, i, k;
                FILE *fp = nullptr;
                png_structp png = nullptr;
                png_infop info = nullptr;
                png_bytep *row_pointers = nullptr;
                bool success = false;
                png_byte color_type = 0;
                png_byte bit_depth = 0;
                size_t width = 0;
                size_t height = 0;

                try {
                        png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
                        if (png == nullptr) {
                                throw std::runtime_error("ImageIO::load_png: png_create_read_struct failed");
                        }
                        // TBD: This isn't happy in release mode.
                        // Replace png code with
                        if (setjmp(png_jmpbuf(png))) {
                                r_err("ImageIO::load_png: setjmp returned error");
                                return false;
                        }

                        info = png_create_info_struct(png);
                        if (info == nullptr) {
                                throw std::runtime_error("ImageIO::load_png: png_create_info_struct failed");
                        }

                        fp = fopen(filename, "rb");
                        if (fp == nullptr) {
                                std::string error = std::string("ImageIO::load_png: Failed to open the file ") + filename;
                                throw std::runtime_error(error);
                        }

                        png_init_io(png, fp);
                        png_read_info(png, info);

                        color_type = png_get_color_type(png, info);
                        bit_depth = png_get_bit_depth(png, info);
                        width = png_get_image_width(png, info);
                        height = png_get_image_height(png, info);

                        // Convert 16-bits to 8-bits
                        if (bit_depth == 16)
                                png_set_strip_16(png);

                        // Convert < 8-bit gray scale to 8-bit
                        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
                                png_set_expand_gray_1_2_4_to_8(png);

                        if (color_type == PNG_COLOR_TYPE_GRAY) {
                                image.init(Image::BW, width, height);

                        } else if (color_type == PNG_COLOR_TYPE_RGB
                                   || color_type == PNG_COLOR_TYPE_RGBA) {
                                image.init(Image::RGB, width, height);

                        } else {
                                throw std::runtime_error("Unsupported PNG image format");
                        }

                        png_read_update_info(png, info);

                        row_pointers = (png_bytep *) r_alloc(sizeof(png_bytep) * height);
                        if (row_pointers == nullptr)
                                throw std::runtime_error("failed to allocate row_pointers mem.");

                        for (y = 0; y < height; y++) {
                                row_pointers[y] = (png_byte *) r_alloc(png_get_rowbytes(png, info));
                                if (row_pointers[y] == nullptr)
                                        throw std::runtime_error(std::string("failed to allocate row_pointers[") + std::to_string(y) + "]");
                        }

                        png_read_image(png, row_pointers);

                        auto& data = image.data();

                        if (color_type == PNG_COLOR_TYPE_GRAY) {
                                for (y = 0, k = 0; y < height; y++) {
                                        png_bytep row = row_pointers[y];
                                        for (x = 0; x < width; x++)
                                                data[k++] = (float) *row++ / 255.0f;
                                }
                        } else if (color_type == PNG_COLOR_TYPE_RGB) {
                                for (y = 0, k = 0; y < height; y++) {
                                        png_bytep row = row_pointers[y];
                                        for (x = 0; x < width; x++) {
                                                data[k++] = (float) *row++ / 255.0f;
                                                data[k++] = (float) *row++ / 255.0f;
                                                data[k++] = (float) *row++ / 255.0f;
                                        }
                                }
                        } else if (color_type == PNG_COLOR_TYPE_RGBA) {
                                for (y = 0, k = 0; y < height; y++) {
                                        png_bytep row = row_pointers[y];
                                        for (x = 0, i = 0; x < width; x++, i += 4) {
                                                float alpha = (float) row[i + 3] / 255.0f;
                                                data[k++] = alpha * (float) row[i] / 255.0f;
                                                data[k++] = alpha * (float) row[i + 1] / 255.0f;
                                                data[k++] = alpha * (float) row[i + 2] / 255.0f;
                                        }
                                }
                        }

                        success = true;
                        
                } catch (std::exception& e) {
                        r_err(e.what());
                }

                if (fp)
                        fclose(fp);

                if (row_pointers) {
                        for (y = 0; y < height; y++)
                                if (row_pointers[y])
                                        r_free(row_pointers[y]);
                        r_free(row_pointers);
                }

                png_destroy_read_struct(&png, &info, nullptr);

                return success;
        }


}
