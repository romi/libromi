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
#include "picamera/CameraComponent.h"

namespace romi::arm {
        
        CameraComponent::CameraComponent(PiCameraSettings& settings)
                : Component(MMAL_COMPONENT_DEFAULT_CAMERA),
                  preview_port_(nullptr),
                  video_port_(nullptr),
                  still_port_(nullptr),
                  shutter_speed_(0)
        {
                assert_settings(settings);
                shutter_speed_ = settings.shutter_speed_;
                
                assert_output_ports();
                
                get_camera_ports();
                enable_control_port();
                
                set_camera_number();
                set_sensor_mode_to_auto();                
                disable_stereo_mode();
                disable_annotation();
                set_parameters(settings);
                set_port_formats(settings);

                enable();
        }

        CameraComponent::~CameraComponent()
        {
                check_disable_port(video_port_);
                check_disable_port(preview_port_);
        }
        
        void CameraComponent::assert_settings(PiCameraSettings& settings)
        {
                if (!settings.is_valid()) {
                        r_err("CameraComponent::assert_settings() failed");
                        throw std::runtime_error("Invalid settings");
                }
        }
        
        void CameraComponent::assert_output_ports()
        {
                if (ptr_->output_num != 3) {
                        r_err("CameraComponent::assert_output_ports() failed");
                        throw std::runtime_error("Camera doesn't have output ports");
                }
        }
        
        void CameraComponent::get_camera_ports()
        {
                preview_port_ = ptr_->output[kPreviewPort];
                video_port_ = ptr_->output[kVideoPort];
                still_port_ = ptr_->output[kCapturePort];
        }
        
        void CameraComponent::handle_control_callback(MMAL_BUFFER_HEADER_T *buffer)
        {
                r_info("Camera control callback  cmd=0x%08x", buffer->cmd);

                if (buffer->cmd == MMAL_EVENT_PARAMETER_CHANGED) {
                        MMAL_EVENT_PARAMETER_CHANGED_T *param
                                = (MMAL_EVENT_PARAMETER_CHANGED_T *) buffer->data;
                        if (param->hdr.id == MMAL_PARAMETER_CAMERA_SETTINGS) {
                                MMAL_PARAMETER_CAMERA_SETTINGS_T *settings
                                        = (MMAL_PARAMETER_CAMERA_SETTINGS_T*) param;
                                r_info("Exposure now %u, analog gain %u/%u, "
                                       "digital gain %u/%u",
                                       settings->exposure,
                                       settings->analog_gain.num,
                                       settings->analog_gain.den,
                                       settings->digital_gain.num,
                                       settings->digital_gain.den);
                                r_info("AWB R=%u/%u, B=%u/%u",
                                       settings->awb_red_gain.num,
                                       settings->awb_red_gain.den,
                                       settings->awb_blue_gain.num,
                                       settings->awb_blue_gain.den);
                        }
                } else if (buffer->cmd == MMAL_EVENT_ERROR) {
                        r_err("No data received from sensor. "
                                "Check all connections.");
                } else {
                        r_err("Received unexpected camera control "
                                "callback event, 0x%08x", buffer->cmd);
                }

                mmal_buffer_header_release(buffer);
        }

        void CameraComponent::set_port_formats(PiCameraSettings& settings)
        {
                set_preview_format(settings.preview_width_,
                                   settings.preview_height_);
                set_video_format();
                set_still_format(settings.width_, settings.height_);
                ensure_number_of_video_buffer(video_port_);
                ensure_number_of_video_buffer(still_port_);
        }

        void CameraComponent::set_preview_format(size_t width, size_t height)
        {
                MMAL_STATUS_T status;
                MMAL_ES_FORMAT_T *format;
                
                format = preview_port_->format;
                format->encoding = MMAL_ENCODING_OPAQUE;
                format->encoding_variant = MMAL_ENCODING_I420;

                if (shutter_speed_ > 6000000) {
                        set_fps_range(preview_port_, 5, 166);
                } else if (shutter_speed_ > 1000000) {
                        set_fps_range(preview_port_, 166, 999);
                }
                
                format->es->video.width = width;
                format->es->video.height = height;
                format->es->video.crop.x = 0;
                format->es->video.crop.y = 0;
                format->es->video.crop.width = (int32_t) width;
                format->es->video.crop.height = (int32_t) height;
                
                /* Frames rates of 0 implies variable, but denominator
                 * needs to be 1 to prevent div by 0. */
                format->es->video.frame_rate.num = 0;
                format->es->video.frame_rate.den = 1;
                
                status = mmal_port_format_commit(preview_port_);
                assert_status("set_preview_format", status);
        }

        void CameraComponent::set_video_format()
        {
                MMAL_STATUS_T status;
                // Set the same format on the video port (which we
                // don't use here) as on the preview port.
                mmal_format_full_copy(video_port_->format, preview_port_->format);
                status = mmal_port_format_commit(video_port_);
                assert_status("set_video_format", status);
        }

        void CameraComponent::ensure_number_of_video_buffer(MMAL_PORT_T *port)
        {
                if (port->buffer_num < kNumberOdVideoOutputBuffers)
                        port->buffer_num = kNumberOdVideoOutputBuffers;
        }
        
        void CameraComponent::set_fps_range(MMAL_PORT_T *port,
                                            int32_t low, int32_t high)
        {
                MMAL_STATUS_T status;
                MMAL_PARAMETER_FPS_RANGE_T fps_range = {
                        { MMAL_PARAMETER_FPS_RANGE, sizeof(fps_range) },
                        { low, 1000 }, { high, 1000 }
                };
                status = mmal_port_parameter_set(port, &fps_range.hdr);
                assert_status("set_fps_range", status);
        }

        void CameraComponent::set_still_format(size_t width, size_t height)
        {
                MMAL_STATUS_T status;
                MMAL_ES_FORMAT_T *format;
                
                format = still_port_->format;

                if (shutter_speed_ > 6000000) {
                        set_fps_range(still_port_, 5, 166);
                } else if(shutter_speed_ > 1000000) {
                        set_fps_range(still_port_, 166, 999);
                }
                
                // Set our stills format on the stills (for encoder) port
                format->encoding = MMAL_ENCODING_OPAQUE;
                format->es->video.width = width;
                format->es->video.height = height;
                format->es->video.crop.x = 0;
                format->es->video.crop.y = 0;
                format->es->video.crop.width = (int32_t) width;
                format->es->video.crop.height = (int32_t) height;
                format->es->video.frame_rate.num = 0;
                format->es->video.frame_rate.den = 1;

                status = mmal_port_format_commit(still_port_);
                assert_status("set_still_format", status);
        }
        
        void CameraComponent::disable_stereo_mode()
        {
                disable_stereo_mode(preview_port_);
                disable_stereo_mode(video_port_);
                disable_stereo_mode(still_port_);
        }

        void CameraComponent::disable_stereo_mode(MMAL_PORT_T *port)
        {
                MMAL_STATUS_T status;
                MMAL_PARAMETER_STEREOSCOPIC_MODE_T stereo = {
                        { MMAL_PARAMETER_STEREOSCOPIC_MODE, sizeof(stereo) },
                        MMAL_STEREOSCOPIC_MODE_NONE, MMAL_FALSE, MMAL_FALSE
                };
                status = mmal_port_parameter_set(port, &stereo.hdr);
                assert_status("disable_stereo_mode", status);
        }
        
        void CameraComponent::disable_annotation()
        {
                MMAL_PARAMETER_CAMERA_ANNOTATE_V4_T annotate;
                memset(&annotate, 0, sizeof(MMAL_PARAMETER_CAMERA_ANNOTATE_V4_T));
                annotate.hdr.id = MMAL_PARAMETER_ANNOTATE;
                annotate.hdr.size = sizeof(MMAL_PARAMETER_CAMERA_ANNOTATE_V4_T);
                annotate.enable = 0;
                set_control_parameter("disable_annotation", &annotate.hdr);
        }
        
        void CameraComponent::set_camera_number()
        {
                MMAL_PARAMETER_INT32_T camera_num = {
                        { MMAL_PARAMETER_CAMERA_NUM, sizeof(camera_num) },
                        kCameraNumber
                };
                set_control_parameter("set_camera_number", &camera_num.hdr);
        }

        void CameraComponent::set_sensor_mode_to_auto()
        {
                set_control_parameter("set_sensor_mode_to_auto",
                                      MMAL_PARAMETER_CAMERA_CUSTOM_SENSOR_CONFIG,
                                      kModeAuto);
        }

        void CameraComponent::set_parameters(PiCameraSettings& settings)
        {
                set_config(settings.width_, settings.height_);
                set_saturation(settings.saturation_);
                set_sharpness(settings.sharpness_);
                set_contrast(settings.contrast_);
                set_brightness(settings.brightness_);
                set_iso(settings.iso_);
                set_video_stabilisation(settings.video_stabilisation_);
                set_exposure_compensation(settings.exposure_compensation_);
                set_exposure_mode(settings.exposure_mode_);
                set_metering_mode(settings.exposure_meter_mode_);
                set_flicker_avoid_mode(settings.flicker_avoid_mode_);
                set_awb_mode(settings.awb_mode_);
                set_awb_gains(settings.awb_gains_r_, settings.awb_gains_b_);
                set_image_fx(settings.image_effect_);
                set_colour_fx(settings.colour_effects_);
                set_rotation(settings.rotation_);
                set_flips(settings.hflip_, settings.vflip_);
                set_roi(settings.roi_);
                set_drc(settings.drc_level_);
                set_stats_pass(settings.stats_pass_);
                set_analog_gain(settings.analog_gain_);
                set_digital_gain(settings.digital_gain_);
                set_focus_window(settings.focus_window_);
        }
        
        void CameraComponent::set_config(size_t width, size_t height)
        {
                MMAL_PARAMETER_CAMERA_CONFIG_T cam_config = {
                        { MMAL_PARAMETER_CAMERA_CONFIG, sizeof(cam_config) },
                        width, // .max_stills_w 
                        height, // .max_stills_h 
                        0, // .stills_yuv422 
                        1, // .one_shot_stills 
                        width, // .max_preview_video_w 
                        height, // .max_preview_video_h 
                        3, // .num_preview_video_frames 
                        0, // .stills_capture_circular_buffer_height 
                        0, // .fast_preview_resume 
                        MMAL_PARAM_TIMESTAMP_MODE_RESET_STC // .use_stc_timestamp 
                };
                set_control_parameter("set_config", &cam_config.hdr);
        }

        void CameraComponent::set_fraction(const char *caller_name,
                                           uint32_t id, int nominator)
        {
                if (nominator >= -100 && nominator <= 100) {
                        MMAL_RATIONAL_T value = { nominator, 100 };
                        set_control_parameter(caller_name, id, value);
                } else {
                        throw std::runtime_error("Invalid value");
                }
        }

        void CameraComponent::set_saturation(int32_t value)
        {
                set_fraction("set_saturation", MMAL_PARAMETER_SATURATION, value);
        }

        void CameraComponent::set_sharpness(int32_t value)
        {
                set_fraction("set_sharpness", MMAL_PARAMETER_SHARPNESS, value);
        }

        void CameraComponent::set_contrast(int32_t value)
        {
                set_fraction("set_contrast", MMAL_PARAMETER_CONTRAST, value);
        }

        void CameraComponent::set_brightness(int32_t value)
        {
                set_fraction("set_brightness", MMAL_PARAMETER_BRIGHTNESS, value);
        }
        
        void CameraComponent::set_iso(uint32_t value)
        {
                set_control_parameter("set_iso", MMAL_PARAMETER_ISO, value);
        }

        void CameraComponent::set_video_stabilisation(bool value)
        {
                set_control_parameter("set_video_stabilisation",
                                      MMAL_PARAMETER_VIDEO_STABILISATION,
                                      value);
        }

        void CameraComponent::set_exposure_compensation(int32_t value)
        {
                set_control_parameter("set_exposure_compensation",
                                      MMAL_PARAMETER_EXPOSURE_COMP,
                                      value);
        }

        void CameraComponent::set_exposure_mode(MMAL_PARAM_EXPOSUREMODE_T value)
        {
                MMAL_PARAMETER_EXPOSUREMODE_T exp_mode = {
                        { MMAL_PARAMETER_EXPOSURE_MODE, sizeof(exp_mode) },
                        value
                };
                set_control_parameter("set_exposure_mode", &exp_mode.hdr);
        }

        void CameraComponent::set_metering_mode(MMAL_PARAM_EXPOSUREMETERINGMODE_T value)
        {
                MMAL_PARAMETER_EXPOSUREMETERINGMODE_T meter_mode = {
                        { MMAL_PARAMETER_EXP_METERING_MODE, sizeof(meter_mode) },
                        value
                };
                set_control_parameter("set_metering_mode", &meter_mode.hdr);
        }

        void CameraComponent::set_awb_mode(MMAL_PARAM_AWBMODE_T value)
        {
                MMAL_PARAMETER_AWBMODE_T mode = {
                        { MMAL_PARAMETER_AWB_MODE, sizeof(mode) },
                        value
                };
                set_control_parameter("set_awb_mode", &mode.hdr);
        }

        void CameraComponent::set_awb_gains(float red, float blue)
        {
                if (red != 0.0 && blue != 0.0) {
                        MMAL_PARAMETER_AWB_GAINS_T param = {
                                { MMAL_PARAMETER_CUSTOM_AWB_GAINS, sizeof(param) },
                                {0, 0}, {0, 0}
                        };
                        param.r_gain.num = (int32_t) (red * 65536);
                        param.r_gain.den = 65536;
                        param.b_gain.num = (int32_t) (blue * 65536);
                        param.b_gain.den = 65536;
                        set_control_parameter("set_awb_gains", &param.hdr);
                }
        }

        void CameraComponent::set_image_fx(MMAL_PARAM_IMAGEFX_T value)
        {
                MMAL_PARAMETER_IMAGEFX_T fx = {
                        { MMAL_PARAMETER_IMAGE_EFFECT, sizeof(fx) },
                        value
                };
                set_control_parameter("set_image_fx", &fx.hdr);
        }

        void CameraComponent::set_colour_fx(ColourEffects& value)
        {
                MMAL_PARAMETER_COLOURFX_T fx = {
                        { MMAL_PARAMETER_COLOUR_EFFECT, sizeof(fx) },
                        0, 0, 0
                };
                fx.enable = value.enable;
                fx.u = value.u;
                fx.v = value.v;
                set_control_parameter("set_colour_fx", &fx.hdr);
        }
        
        void CameraComponent::set_flicker_avoid_mode(MMAL_PARAM_FLICKERAVOID_T value)
        {
                MMAL_PARAMETER_FLICKERAVOID_T mode = {
                        { MMAL_PARAMETER_FLICKER_AVOID, sizeof(mode) },
                        value
                };
                set_control_parameter("set_flicker_avoid_mode", &mode.hdr);
        }

        void CameraComponent::set_rotation(int32_t value)
        {
                int32_t rotation = ((value % 360 ) / 90) * 90;

                mmal_port_parameter_set_int32(ptr_->output[0],
                                              MMAL_PARAMETER_ROTATION,
                                              rotation);
                mmal_port_parameter_set_int32(ptr_->output[1],
                                              MMAL_PARAMETER_ROTATION,
                                              rotation);
                mmal_port_parameter_set_int32(ptr_->output[2],
                                              MMAL_PARAMETER_ROTATION,
                                              rotation);
        }

        void CameraComponent::set_flips(bool hflip, bool vflip)
        {
                MMAL_PARAMETER_MIRROR_T mirror = {
                        { MMAL_PARAMETER_MIRROR, sizeof(MMAL_PARAMETER_MIRROR_T) },
                        MMAL_PARAM_MIRROR_NONE
                };

                if (hflip && vflip)
                        mirror.value = MMAL_PARAM_MIRROR_BOTH;
                else if (hflip)
                        mirror.value = MMAL_PARAM_MIRROR_HORIZONTAL;
                else if (vflip)
                        mirror.value = MMAL_PARAM_MIRROR_VERTICAL;

                mmal_port_parameter_set(ptr_->output[0], &mirror.hdr);
                mmal_port_parameter_set(ptr_->output[1], &mirror.hdr);
                mmal_port_parameter_set(ptr_->output[2], &mirror.hdr);
        }

        void CameraComponent::set_roi(FloatRectangle& roi)
        {
                MMAL_PARAMETER_INPUT_CROP_T crop = {
                        { MMAL_PARAMETER_INPUT_CROP,
                          sizeof(MMAL_PARAMETER_INPUT_CROP_T) },
                        { 0, 0, 0, 0 }
                };                
                crop.rect.x = (int32_t) (65536.0f * roi.x);
                crop.rect.y = (int32_t) (65536.0f * roi.y);
                crop.rect.width = (int32_t) (65536.0f * roi.w);
                crop.rect.height = (int32_t) (65536.0f * roi.h);
                set_control_parameter("set_roi", &crop.hdr);
        }

        void CameraComponent::set_drc(MMAL_PARAMETER_DRC_STRENGTH_T value)
        {
                MMAL_PARAMETER_DRC_T drc = {
                        { MMAL_PARAMETER_DYNAMIC_RANGE_COMPRESSION,
                          sizeof(MMAL_PARAMETER_DRC_T) },
                        value
                };
                set_control_parameter("set_drc", &drc.hdr);
        }

        void CameraComponent::set_stats_pass(bool value)
        {
                set_control_parameter("set_stats_pass",
                                      MMAL_PARAMETER_CAPTURE_STATS_PASS,
                                      value);
        }
        
        void CameraComponent::set_analog_gain(float value)
        {
                MMAL_RATIONAL_T rational = {0, 65536};
                rational.num = (int32_t) (value * 65536);
                set_control_parameter("set_analog_gain",
                                      MMAL_PARAMETER_ANALOG_GAIN,
                                      rational);
        }
        
        void CameraComponent::set_digital_gain(float value)
        {
                MMAL_RATIONAL_T rational = {0, 65536};
                rational.num = (int32_t)(value * 65536);
                set_control_parameter("set_digital_gain",
                                      MMAL_PARAMETER_DIGITAL_GAIN,
                                      rational);
        }
        
        void CameraComponent::set_focus_window(bool value)
        {
                set_control_parameter("set_focus_window",
                                      MMAL_PARAMETER_DRAW_BOX_FACES_AND_FOCUS,
                                      value);
        }

        void CameraComponent::set_shutter_speed()
        {
                set_shutter_speed(shutter_speed_);
        }

        void CameraComponent::set_shutter_speed(uint32_t value)
        {
                set_control_parameter("set_shutter_speed",
                                      MMAL_PARAMETER_SHUTTER_SPEED,
                                      value);
        }

        void CameraComponent::trigger_capture()
        {
                MMAL_STATUS_T status;
                status = mmal_port_parameter_set_boolean(still_port_,
                                                    MMAL_PARAMETER_CAPTURE,
                                                    1);
                if (status != MMAL_SUCCESS) {
                        r_err("%s: Failed to start capture");
                        throw std::runtime_error("Failed to start capture");
                }
        }
}

