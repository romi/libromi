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

#ifndef __ROMI_IMAGE_H
#define __ROMI_IMAGE_H

#include <stdexcept>
#include <vector>

namespace romi {

        class Image
        {
        public:
                enum ImageType { BW, RGB};
                
        protected:
                size_t _width;
                size_t _height;
                ImageType _type;
                size_t _channels;
                std::vector<float> _data;

                void import_data(const uint8_t *data);
                void do_init(ImageType type, size_t width, size_t height);

        public:
                Image();
                Image(ImageType type, size_t width, size_t height);
                Image(ImageType type, const uint8_t *data, size_t width, size_t height);
                virtual ~Image();
                Image& operator=(const Image&);


                // Accessors
                ImageType type() {
                        return _type;
                }
                
                size_t width() const {
                        return _width;
                }
                
                size_t height() const {
                        return _height;
                }
                
                size_t channels() const {
                        return _channels;
                }
                
                std::vector<float>& data() {
                        return _data;
                }
                
                bool contains(size_t x, size_t y) const {
                        return (x < width() && y < height());
                }

                size_t offset(size_t channel, size_t x, size_t y) const {
                        return (y * channels() * width() + x * channels() + channel);
                }

                void set(size_t channel, size_t x, size_t y, float color) {
                        if (contains(x, y)) {
                                // TBD: Not safe Offset not calculated in contains()
                                _data[offset(channel, x, y)] = color;
                        }
                }
                
                float get(size_t channel, size_t x, size_t y) {
                        float value = 0.0f;
                        if (contains(x, y)) {
                                value = _data[offset(channel, x, y)];
                        }
                        return value;
                }

                std::vector<uint8_t> export_byte_data();
                
                // Operations
                void init(ImageType type, size_t width, size_t height);
                void import(ImageType type, const uint8_t *data,
                            size_t width, size_t height);
                void fill(size_t channel, float color);
                void crop(size_t x, size_t y, size_t width, size_t height, Image &out);
                void scale(size_t n, Image &out);

                size_t length();
                size_t byte_length();
        };
}

#endif // __ROMI_IMAGE_H
