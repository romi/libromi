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

#ifndef __ROMI_IMAGE_CROPPER_H
#define __ROMI_IMAGE_CROPPER_H

#include "IImageCropper.h"

namespace romi {
        
        class ImageCropper : public IImageCropper
        {
        protected:
                CNCRange _range;
                size_t _x0;
                size_t _y0;
                size_t _width;
                size_t _height;

                void set_workspace(JsonCpp value);
                void assert_workspace_dimensions();

        public:
                ImageCropper(CNCRange& range, JsonCpp& properties);
                ~ImageCropper() override = default;
                
                bool crop(ISession &session,
                          Image &camera_image,
                          double tool_diameter,
                          Image &cropped_image) override;

                double map_meters_to_pixels(double meters) override;
        };
}

#endif // __ROMI_IMAGE_CROPPER_H
