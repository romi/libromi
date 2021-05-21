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

#ifndef __ROMI_CV_H
#define __ROMI_CV_H

#include <vector>
#include <utility>
#include "cv/Image.h"
#include "v3.h"

namespace romi {

        using Center = std::pair<uint32_t, uint32_t>;
        using Centers = std::vector<Center>;
        
        bool segment_crosses_white_area(const Image& mask, v3 start, v3 end);

        // in and out can be the same image.
        void filter_mask(Image &in, Image &out, size_t n);

        void compute_connected_components(Image &mask, Image &components);

        Centers calculate_centers(Image &mask, size_t max_centers);
                
        std::vector<Centers> sort_centers(Centers& centers, Image& components);

    std::vector<std::pair<uint32_t, uint32_t>>
    calculate_adjacent_centres(const std::vector<double> &kseedsl,
                               const std::vector<double> &kseedsx, const std::vector<double> &kseedsy);
        
}

#endif // __ROMI_CV_H
