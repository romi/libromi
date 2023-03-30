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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdexcept>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <i2c/smbus.h>
#include <i2c/smbus.h>

#include "util/Logger.h"
#include "util/ClockAccessor.h"
#include "hal/I2C.h"

namespace romi {

        I2C::I2C(rcom::ILinux& linux, const char *device, int address)
                : linux_(linux),
                  device_(device),
                  address_(address),
                  fd_(-1)
        {
                fd_ = linux_.open(device, O_RDWR);
                if (fd_ < 0) {
                        r_err("I2C::I2C: Failed to open %s", device);
                        throw std::runtime_error("I2C::I2C: Failed to open");
                }

                //if (linux_.ioctl(fd_, I2C_SLAVE, data) < 0) {
                if (ioctl(fd_, I2C_SLAVE, address) < 0) {
                        r_err("I2C::I2C: Unable to get bus access to talk to slave");
                        linux_.close(fd_);
                        throw std::runtime_error("I2C::I2C: ioctl failed");
                }
        }

        I2C::~I2C()
        {
                if (fd_ >= 0) {
                        linux_.close(fd_);
                }
        }
                
        int32_t I2C::get(uint8_t register_)
        {
                receive(register_);
                uint8_t checksum = compute_checksum();
                assure_checksum(checksum);
                return from_buffer();
        }
        
        void I2C::set(uint8_t register_, int32_t value)
        {
                to_buffer(value);
                uint8_t checksum = compute_checksum();
                set_checksum(checksum);
                send(register_);
        }

        uint8_t I2C::compute_checksum()
        {
                uint32_t sum = 0;
                for (int i = 0; i < kCheckSumIndex; i++) 
                        sum += buffer_[i];
                return (uint8_t) (sum & 0x000000ff);
        }
        
        void I2C::assure_checksum(uint8_t sum)
        {
                if (buffer_[kCheckSumIndex] != sum) {
                        r_err("I2C::assure_checksum: %d != %d", (int) sum,
                              (int) buffer_[kCheckSumIndex]);
                        throw std::runtime_error("I2C::assure_checksum");
                }
        }

        void I2C::set_checksum(uint8_t sum)
        {
                buffer_[kCheckSumIndex] = sum; 
        }

        void I2C::to_buffer(int32_t value)
        {
                buffer_[0] =  (uint8_t) (value & (int32_t) 0x000000ff);
                buffer_[1] = (uint8_t) ((value & (int32_t) 0x0000ff00) >> 8);
                buffer_[2] = (uint8_t) ((value & (int32_t) 0x00ff0000) >> 16);
                buffer_[3] = (uint8_t) ((value & (int32_t) 0xff000000) >> 24);
        }
        
        int32_t I2C::from_buffer()
        {
                int32_t value = ((int32_t) buffer_[0]
                                 | (int32_t) buffer_[1] << 8
                                 | (int32_t) buffer_[2] << 16
                                 | (int32_t) buffer_[3] << 24);
                return value;
        }
        
        void I2C::send(uint8_t register_)
        {
                int32_t err;
                err = linux_.i2c_smbus_write_block_data(fd_, register_,
                                                        kBufferLength, buffer_);
                if (err != 0) {
                        r_err("I2C::I2C: Failed to write the data");
                        throw std::runtime_error("I2C::receive: Failed to write the data");
                }
                sleep();
        }
        
        void I2C::receive(uint8_t register_)
        {
                int32_t n;
                n = linux_.i2c_smbus_read_block_data(fd_, register_,
                                                     kBufferLength, buffer_);
                if (n < 0) {
                        r_err("I2C::receive: Failed to read the data");
                        throw std::runtime_error("I2C::receive: Failed to read the data");
                }
                if (n < (int32_t) kBufferLength) {
                        r_err("I2C::receive: Failed to read the data, got %d/%d bytes",
                              (int) n, (int) kBufferLength);
                        throw std::runtime_error("I2C::receive: Failed to read the data");
                }
                sleep();
        }

        void I2C::sleep()
        {
                auto clock = romi::ClockAccessor::GetInstance();
                clock->sleep(0.010);
        }
}
