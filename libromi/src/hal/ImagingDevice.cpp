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

#include "hal/ImagingDevice.h"

namespace romi {
        
        ImagingDevice::ImagingDevice(std::shared_ptr<ICamera>& camera,
                                     std::shared_ptr<ICameraMount>& mount)
                : camera_(camera),
                  mount_(mount)
        {
        }
        
        bool ImagingDevice::grab(Image &image)
        {
                return camera_->grab(image);
        }
        
        rcom::MemBuffer& ImagingDevice::grab_jpeg()
        {
                return camera_->grab_jpeg();
        }
        
        bool ImagingDevice::set_value(const std::string& name, double value)
        {
                return camera_->set_value(name, value);
        }
        
        bool ImagingDevice::select_option(const std::string& name,
                                          const std::string& value)
        {
                return camera_->select_option(name, value);
        }
        
        bool ImagingDevice::get_range(CNCRange &xyz, IRange &angles)
        {
                return mount_->get_range(xyz, angles);
        }
        
        bool ImagingDevice::get_position(v3& xyz, v3& angles)
        {
                return mount_->get_position(xyz, angles);
        }
        
        bool ImagingDevice::synchronize(double timeout_seconds)
        {
                return mount_->synchronize(timeout_seconds);
        }
        
        bool ImagingDevice::homing()
        {
                return mount_->homing();
        }

        uint8_t ImagingDevice::count_relays()
        {
                return mount_->count_relays();
        }
        
        bool ImagingDevice::set_relay(uint8_t index, bool value)
        {
                return mount_->set_relay(index, value);
        }
        
        bool ImagingDevice::moveto(double x, double y, double z,
                                   double phi_x, double phi_y, double phi_z,
                                   double relative_speed)
        {
                return mount_->moveto(x, y, z, phi_x, phi_y, phi_z, relative_speed);
        }

        bool ImagingDevice::pause_activity()
        {
                return mount_->pause_activity();
        }
        
        bool ImagingDevice::continue_activity()
        {
                return mount_->continue_activity();
        }
        
        bool ImagingDevice::reset_activity()
        {
                return mount_->reset_activity();
        }

        bool ImagingDevice::power_up()
        {
                return camera_->power_up()
                        && mount_->power_up();
        }
        
        bool ImagingDevice::power_down()
        {
                return camera_->power_down()
                        && mount_->power_down();
        }
        
        bool ImagingDevice::stand_by()
        {
                return camera_->stand_by()
                        && mount_->stand_by();
        }
        
        bool ImagingDevice::wake_up()
        {
                return camera_->wake_up()
                        && mount_->wake_up();
        }

        const ICameraSettings& ImagingDevice::get_settings()
        {
                return camera_->get_settings();
        }
}
