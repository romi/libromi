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

#include <math.h>
#include <stdexcept>
#include <r.h>
#include "rover/L1NavigationController.h"

namespace romi {
        
        L1NavigationController::L1NavigationController(double width, double L)
                : w_(width), L_(L) {
                assert_valid_parameters();
        }

        void L1NavigationController::assert_valid_parameters()
        {
                assert_valid_width();
                assert_valid_distance();
        }

        void L1NavigationController::assert_valid_width()
        {
                if (w_ <= 0.0) {
                        r_err("L1NavigationController: The width should be > 0: %f ", w_);
                        throw std::runtime_error("L1NavigationController: Invalid width");
                }
                if (w_ > 5.0) {
                        r_err("L1NavigationController: Please verify the width, "
                              "should be < 5 m: %f ", w_);
                        throw std::runtime_error("L1NavigationController: Invalid width");
                }
        }

        void L1NavigationController::assert_valid_distance()
        {
                if (L_ < w_) {
                        r_err("L1NavigationController: L1 should be at least equal "
                              "to the width: %f < %f", L_, w_);
                        throw std::runtime_error("L1NavigationController: Invalid L1");
                }
                if (L_ > 20.0) {
                        r_err("L1NavigationController: Please verify the value of L1."
                              "The current value is high: %d > 20m", L_);
                        throw std::runtime_error("L1NavigationController: Invalid L1");
                }
        }
        
        double L1NavigationController::estimate_correction(double d, double phi) 
        {
                if (d >= L_) {
                        r_err("L1NavigationController: d>=L");
                        throw std::runtime_error("L1NavigationController: d>=L");
                }
                
                double correction = 0.0;
                double theta = phi + atan(d / sqrt(L_ * L_ - d * d));
                if (theta != 0) {
                        double R = -L_ / (2.0 * sin(theta));
                        correction = -w_ / (2.0 * R);
                }
                return correction;
        }
}
