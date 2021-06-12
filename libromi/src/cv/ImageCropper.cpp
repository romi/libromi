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
#include "cv/ImageCropper.h"

namespace romi {

        ImageCropper::ImageCropper(CNCRange& range, JsonCpp& properties)
                : range_(range), x0_(0), y0_(0), width_(0), height_(0)
        {
                try {
                        set_workspace(properties["workspace"]);
                        
                } catch (JSONError& je) {
                        r_err("ImageCropper: Failed to parse the workspace dimensions: %s",
                              je.what());
                        throw std::runtime_error("ImageCropper: bad config");
                }
        } 

        void ImageCropper::assert_workspace_dimensions()
        {
                if ((x0_ > 10000)
                    || (y0_ > 10000)
                    || (width_ > 10000)
                    || (height_ > 10000)) {
                        r_err("ImageCropper: Invalid workspace values: "
                              "workspace: x0 %d, y0 %d, width %d px, height %d px", 
                              x0_, y0_, width_, height_);
                        throw std::runtime_error("ImageCropper: bad workspace dimensions");
                }
        }
        
        void ImageCropper::set_workspace(JsonCpp w)
        {
                x0_ = (size_t)w.num(0);
                y0_ = (size_t)w.num(1);
                width_ = (size_t)w.num(2);
                height_ = (size_t)w.num(3);
                assert_workspace_dimensions();

                r_debug("workspace: x0 %d, y0 %d, width %d px, height %d px", 
                        x0_, y0_, width_, height_);
        }

        double ImageCropper::map_meters_to_pixels(double meters)
        {
                printf("x[0]=%f, x[1]=%f\n", range_.min.x(), range_.max.x());
                v3 dimensions = range_.dimensions();
                double meters_to_pixels = (double) width_ / dimensions.x();
                return meters * meters_to_pixels;
        }

        bool ImageCropper::crop(ISession &session,
                                Image &camera,
                                double tool_diameter,
                                Image &out)
        {
                bool success = false;

                camera.crop(x0_, camera.height() - y0_ - height_, width_, height_, out);
                session.store_jpg("workspace", out);

                double diameter = map_meters_to_pixels(tool_diameter);
                size_t border = (size_t) (diameter / 2.0);
                ssize_t x0 = (ssize_t) (x0_ - border);
                size_t width = width_ + 2 * border;
                ssize_t y0 = (ssize_t) (camera.height() - y0_ - height_ - border);
                size_t height = height_ + 2 * border;
                
                session.store_jpg("camera", camera);

                if (x0 < 0) {
                        r_err("ImageCropper::crop: camera position is not good: "
                              "the bottom is cut off");
                } else if (width > camera.width()) {
                        r_err("ImageCropper::crop: camera image width too small");
                } else if (y0 < 0) {
                        r_err("ImageCropper::crop: camera position is not good: "
                              "the top is cut off");
                } else if (height > camera.height()) {
                        r_err("ImageCropper::crop: camera image height too small");
                                
                } else {
                        camera.crop((size_t) x0, (size_t) y0, width, height, out);
                        session.store_jpg("cropped_debug", out);
                        success = true;
                }
                
                return success;
        }
}
