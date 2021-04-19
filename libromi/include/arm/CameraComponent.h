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
#ifndef _LIBROMI_CAMERA_COMPONENT_H_
#define _LIBROMI_CAMERA_COMPONENT_H_

#include "Component.h"
#include "PiCameraSettings.h"

namespace romi::arm {
        
        class CameraComponent : public Component {
        protected:
                static const int kPreviewPort = 0;
                static const int kVideoPort = 1;
                static const int kCapturePort = 2;

                static const int kCameraNumber = 0;

                static const int kModeAuto = 0;
                static const int kNumberOdVideoOutputBuffers = 3;
                
                MMAL_PORT_T *preview_port_;
                MMAL_PORT_T *video_port_;
                MMAL_PORT_T *still_port_;
                uint32_t shutter_speed_;
                
        public:
                
                CameraComponent(PiCameraSettings& settings);
                ~CameraComponent() override;
                
                void trigger_capture();
                void set_shutter_speed();
                
                MMAL_PORT_T *get_still_port() {
                        return still_port_;
                }


        protected:
                void handle_control_callback(MMAL_BUFFER_HEADER_T *buffer) override;
                
                void assert_settings(PiCameraSettings& settings);
                void assert_output_ports();
                void get_camera_ports();
                void set_port_formats(PiCameraSettings& settings);
                void set_preview_format(size_t width, size_t height);
                void set_video_format();
                void ensure_number_of_video_buffer(MMAL_PORT_T *port);
                void set_fps_range(MMAL_PORT_T *port,
                                   int32_t low, int32_t high);
                void set_still_format(size_t width, size_t height);
                void disable_stereo_mode();
                void disable_stereo_mode(MMAL_PORT_T *port);
                void disable_annotation();
                void set_camera_number();
                void set_sensor_mode_to_auto();
                
                void set_parameters(PiCameraSettings& settings);
                
                void set_config(size_t width, size_t height);
                void set_saturation(int32_t value);
                void set_sharpness(int32_t value);
                void set_contrast(int32_t value);
                void set_brightness(int32_t value);
                void set_iso(uint32_t value);
                void set_video_stabilisation(bool value);
                void set_exposure_compensation(int32_t value);
                void set_exposure_mode(MMAL_PARAM_EXPOSUREMODE_T value);
                void set_metering_mode(MMAL_PARAM_EXPOSUREMETERINGMODE_T value);
                void set_awb_mode(MMAL_PARAM_AWBMODE_T value);
                void set_awb_gains(float red, float blue);
                void set_image_fx(MMAL_PARAM_IMAGEFX_T value);
                void set_colour_fx(ColourEffects& value);
                void set_flicker_avoid_mode(MMAL_PARAM_FLICKERAVOID_T value);
                void set_rotation(int32_t value);
                void set_flips(bool hflip, bool vflip);
                void set_roi(FloatRectangle& value);
                void set_drc(MMAL_PARAMETER_DRC_STRENGTH_T value);
                void set_stats_pass(bool value);
                void set_analog_gain(float value);
                void set_digital_gain(float value);
                void set_focus_window(bool value);
                void set_shutter_speed(uint32_t value);
                void set_fraction(const char *caller_name,
                                  uint32_t id, int nominator);
                
        private:
                CameraComponent(CameraComponent& other) = default;
                CameraComponent& operator=(const CameraComponent& other) = default;
        };
}

#endif // _LIBROMI_CAMERA_COMPONENT_H_
