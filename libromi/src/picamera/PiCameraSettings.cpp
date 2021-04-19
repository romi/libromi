/*
  libromi

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  libromi is the base library for the Romi devices.

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
#include <r.h>
#include "PiCameraSettings.h"

namespace romi {
        
        PiCameraSettings::PiCameraSettings()
                : max_width_(640),
                  max_height_(480),
                  width_(640),
                  height_(480),
                  preview_width_(640),
                  preview_height_(480),
                  sharpness_(0),
                  contrast_(0),
                  brightness_(50),
                  saturation_(0),
                  iso_(0),
                  video_stabilisation_(false),
                  exposure_compensation_(0),
                  exposure_mode_(MMAL_PARAM_EXPOSUREMODE_AUTO),
                  exposure_meter_mode_(MMAL_PARAM_EXPOSUREMETERINGMODE_AVERAGE),
                  awb_mode_(MMAL_PARAM_AWBMODE_AUTO),
                  awb_gains_r_(0.0),
                  awb_gains_b_(0.0),
                  image_effect_(MMAL_PARAM_IMAGEFX_NONE),
                  colour_effects_(),
                  flicker_avoid_mode_(MMAL_PARAM_FLICKERAVOID_OFF),
                  rotation_(0),
                  hflip_(false),
                  vflip_(false),
                  roi_(),
                  drc_level_(MMAL_PARAMETER_DRC_STRENGTH_OFF),
                  stats_pass_(false),
                  focus_window_(false),
                  analog_gain_(1.0),
                  digital_gain_(1.0),
                  shutter_speed_(0)
        {
                colour_effects_.enable = false;
                colour_effects_.u = 128;
                colour_effects_.v = 128;
                roi_.x = 0.0;
                roi_.y = 0.0;
                roi_.w = 1.0;
                roi_.h = 1.0;
        }

        bool PiCameraSettings::is_valid()
        {
                return (is_resolution_valid(width_, height_)
                        && is_preview_valid(preview_width_, preview_height_)
                        && is_saturation_valid(saturation_)
                        && is_sharpness_valid(sharpness_)
                        && is_contrast_valid(contrast_)
                        && is_brightness_valid(brightness_)
                        && is_iso_valid(iso_)
                        && is_exposure_compensation_valid(exposure_compensation_)
                        && is_roi_valid(roi_.x, roi_.y, roi_.w, roi_.h)
                        && is_shutter_speed_valid(shutter_speed_)
                        && are_awb_gains_valid(awb_gains_r_, awb_gains_b_)
                        && is_analog_gain_valid(analog_gain_)
                        && is_digital_gain_valid(digital_gain_)
                        && is_colour_effects_valid(colour_effects_.u,  colour_effects_.v));
        }

        bool PiCameraSettings::is_resolution_valid(size_t width, size_t height)
        {
                bool valid = (width <= max_width_ && height <= max_height_);
                if (!valid)
                        r_warn("Invalid resolution");
                return valid;
        }

        bool PiCameraSettings::is_preview_valid(size_t width, size_t height)
        {
                bool valid = (width <= max_width_ && height <= max_height_);
                if (!valid)
                        r_warn("Invalid preview resolution");
                return valid;
        }
        
        bool PiCameraSettings::is_saturation_valid(int32_t saturation)
        {
                bool valid = (-100 <= saturation && saturation <= 100);
                if (!valid)
                        r_warn("Invalid saturation");
                return valid;
        }

        bool PiCameraSettings::is_sharpness_valid(int32_t sharpness)
        {
                bool valid = (-100 <= sharpness && sharpness <= 100);
                if (!valid)
                        r_warn("Invalid sharpness");
                return valid;
        }

        bool PiCameraSettings::is_contrast_valid(int32_t contrast)
        {
                bool valid = (-100 <= contrast && contrast <= 100);
                if (!valid)
                        r_warn("Invalid contrast");
                return valid;
        }

        bool PiCameraSettings::is_brightness_valid(int32_t brightness)
        {
                bool valid = (0 <= brightness && brightness <= 100);
                if (!valid)
                        r_warn("Invalid brightness");
                return valid;
        }

        bool PiCameraSettings::is_iso_valid(uint32_t iso)
        {
                bool valid = (iso <= 10000); // ?
                if (!valid)
                        r_warn("Invalid ISO");
                return valid;
        }

        bool PiCameraSettings::is_exposure_compensation_valid(int32_t value)
        {
                bool valid = (-25 <= value && value <= 25);
                if (!valid)
                        r_warn("Invalid exposure compensation");
                return valid;
        }

        bool PiCameraSettings::is_roi_valid(float x, float y, float w, float h)
        {
                bool valid = ((0.0 <= x && x < 1.0)
                        && (0.0 <= y && y < 1.0)
                        && (0.0 < w && w <= 1.0 - x)
                        && (0.0 < h && h <= 1.0 - y));
                if (!valid)
                        r_warn("Invalid ROI");
                return valid;
        }

        bool PiCameraSettings::is_shutter_speed_valid(uint32_t speed)
        {
                bool valid = (speed < 60*1000);
                if (!valid)
                        r_warn("Invalid shutter speed");
                return valid;
        }

        bool PiCameraSettings::are_awb_gains_valid(float red, float blue)
        {
                bool valid = ((0.0 <= red && red <= 8.0)
                        && (0.0 <= blue && blue <= 8.0));
                if (!valid)
                        r_warn("Invalid AWB gains");
                return valid;
        }

        bool PiCameraSettings::is_analog_gain_valid(float value)
        {
                bool valid = (1.0 <= value && value <= 12.0);
                if (!valid)
                        r_warn("Invalid analog gain");
                return valid;
        }

        bool PiCameraSettings::is_digital_gain_valid(float value)
        {
                bool valid = (1.0 <= value && value <= 64.0);
                if (!valid)
                        r_warn("Invalid ditigal gain");
                return valid;
        }

        bool PiCameraSettings::is_colour_effects_valid(uint32_t u,  uint32_t v)
        {
                bool valid = (u <= 255 && v <= 255);
                if (!valid)
                        r_warn("Invalid colour effect");
                return valid;
        }

        V2CameraSettings::V2CameraSettings(size_t width, size_t height)
                : PiCameraSettings()
        {
                max_width_ = kV2FullWidth;
                max_height_ = kV2FullHeight;
                width_ = width;
                height_ = height;
                preview_width_ = width;
                preview_height_ = height;
        }
        
        HQCameraSettings::HQCameraSettings(size_t width, size_t height)
                : PiCameraSettings()
        {
                max_width_ = kHQFullWidth;
                max_height_ = kHQFullHeight;
                width_ = width;
                height_ = height;
                preview_width_ = width;
                preview_height_ = height;
        }
}
