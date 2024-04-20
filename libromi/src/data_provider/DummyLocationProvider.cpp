/*
  romi-rover

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Douglas Boari, Peter Hanappe

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
#include <rcom/json.hpp>
#include "data_provider/JsonFieldNames.h"
#include "data_provider/DummyLocationProvider.h"
#include "util/Logger.h"

namespace romi {

        DummyLocationProvider::DummyLocationProvider()
                : location_(),
                  position_(0.0)
        {
            location_ = nlohmann::json::object({
                            { JsonFieldNames::x_position.data(), position_.x() },
                            { JsonFieldNames::y_position.data(), position_.y() },
                            { JsonFieldNames::z_position.data(), position_.z() }
                            
                    });
        }

        bool DummyLocationProvider::update_location_estimate()
        {
                return true;
        }

        nlohmann::json DummyLocationProvider::location()
        {
                return location_;
        }

        v3 DummyLocationProvider::coordinates()
        {
                return position_;
        }
}
