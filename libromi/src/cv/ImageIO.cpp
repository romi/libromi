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

#include <png.h>
#include <r.h>
#include <FileUtils.h>
#include "cv/ImageIO.h"
#include "cv/stb_includes.h"

namespace romi {




        bool ImageIO::store_jpg(Image& image, const char *filename)
        {
                auto image_bytes = image.export_byte_data();
                auto write =  stbi_write_jpg(filename, (int)image.width(), (int)image.height(), (int)image.channels(), image_bytes.data(), JPEG_QUALITY_90);
                bool retval = (write != 0);
                return retval;
        }
        
        bool ImageIO::store_png(Image& image, const char *filename)
        {
            auto image_bytes = image.export_byte_data();
            int stride = (int)(image.width() * image.channels());
            auto write =  stbi_write_png(filename, (int)image.width(), (int)image.height(), (int)image.channels(), image_bytes.data(), stride);
            bool retval = (write != 0);
            return retval;
        }

        // Loads an image. Converts to BW or RGB if alpha channel is included in original.
        bool ImageIO::load(Image& image, const char *filename)
        {
            bool success = true;
            int width, height, channels = 0;

            uint8_t* rgb_image = stbi_load(filename, &width, &height, &channels, STBI_default);

            // unlhandled re-load as BW
            if (channels == STBI_grey_alpha)
            {
                stbi_image_free(rgb_image);
                rgb_image = stbi_load(filename, &width, &height, &channels, STBI_grey);
            }

            // unlhandled re-load as RGB
            if (channels == STBI_rgb_alpha)
            {
                stbi_image_free(rgb_image);
                rgb_image = stbi_load(filename, &width, &height, &channels, STBI_rgb);
            }

            if(rgb_image)
            {
                Image::ImageType type = (channels == STBI_grey) ? Image::BW : Image::RGB;
                image.import(type, rgb_image, (size_t)width, (size_t)height);
                stbi_image_free(rgb_image);
            }
            else
                success = false;

            return success;
        }

    bool ImageIO::load_from_buffer(Image& image, const std::vector<uint8_t>& image_data)
    {
        bool success = true;
        int width, height, channels = 0;

        uint8_t* rgb_image = stbi_load_from_memory(image_data.data(), (int)image_data.size(), &width, &height, &channels, STBI_default);

        if (channels == STBI_grey_alpha)
        {
            stbi_image_free(rgb_image);
            rgb_image = stbi_load_from_memory(image_data.data(), (int)image_data.size(), &width, &height, &channels, STBI_grey);
        }

        // unlhandled re-load as RGB
        if (channels == STBI_rgb_alpha)
        {
            stbi_image_free(rgb_image);
            rgb_image = stbi_load_from_memory(image_data.data(), (int)image_data.size(), &width, &height, &channels, STBI_rgb);
        }

        if(rgb_image)
        {
            Image::ImageType type = (channels == STBI_grey) ? Image::BW : Image::RGB;
            image.import(type, rgb_image, (size_t)width, (size_t)height);
            stbi_image_free(rgb_image);
        }
        else
            success = false;

        return success;
    }


}
