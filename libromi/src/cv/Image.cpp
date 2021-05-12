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
#include <cstring>
#include "cv/Image.h"

namespace romi {

        static size_t channels_per_type(Image::ImageType type)
        {
                size_t r = 0;
                switch (type) {
                case Image::BW:
                        r = 1;
                        break;
                case Image::RGB:
                        r = 3;
                        break;
                default:
                        r = 3;
                        break;
                }
                return r;
        }
        
        Image::Image() : _width(0), _height(0), _type(RGB), _channels(3), _data(0)
        {
        }

        Image::Image(ImageType type, size_t width, size_t height)
                : _width(width), _height(height), _type(type), _channels(3), _data()
        { 
                do_init(type, width, height);
        }
        
        Image::Image(ImageType type, const uint8_t *data, size_t width, size_t height)
                : _width(width), _height(height), _type(type), _channels(3),  _data()
        {
                do_init(type, width, height);
                import_data(data);
        }
        
        Image::~Image()
        {
                _data.clear();
        }

        void Image::do_init(ImageType type, size_t width, size_t height)
        {
                _type = type;
                _channels = channels_per_type(type);
                _width = width;
                _height = height;
                _data.resize(length(), 0.0);
        }

        void Image::init(ImageType type, size_t width, size_t height)
        {
                if (_type != type || _width != width || _height != height) {
                        do_init(type, width, height);
                }
        }

        void Image::import(ImageType type, const uint8_t *data,
                           size_t width, size_t height)
        {
                init(type, width, height);
                import_data(data);
        }

        void Image::import_data(const uint8_t *data)
        {
                _data.clear();
                size_t len = length();
                for (size_t i = 0; i < len; i++)
                        _data.emplace_back((float) *data++ / 255.0f);
        }
        
        void Image::fill(size_t channel, float color)
        {
                size_t stride = _channels;
                size_t len = length();
                for (size_t i = channel; i < len; i += stride)
                        _data[i] = color;
        }
        
        void Image::crop(size_t x, size_t y, size_t width, size_t height, Image &out)
        {
                if (x >= _width) {
                        width = 0;
                } else if (x + width > _width) {
                        width = _width - x;
                }
                
                if (y >= _height) {
                        height = 0;
                } else if (y + height > _height) {
                        height = _height - y;
                }
                
                out.init(_type, width, height);
                
                for (size_t line = 0; line < height; line++) {
                        size_t len = width * _channels * sizeof(float);
                        size_t crop_offset = out.offset(0, 0, line);
                        size_t im_offset = offset(0, x, y + line);
                        memcpy(&out._data[crop_offset], &_data[im_offset], len); 
                }
        }

        void Image::scale(size_t n, Image &out)
        {
                if (n == 0)
                        n = 1;
                
                size_t width = (_width + n - 1) / n;
                size_t height = (_height + n - 1) / n;
                // size_t offx = n / 2;
                // size_t offy = n / 2;
                
                out.init(_type, width, height);

                for (size_t y = 0; y < height; y++) {
                        for (size_t x = 0; x < width; x++) {
                                size_t i0 = offset(0, n * x, n * y);
                                size_t i1 = out.offset(0, x, y);
                                for (size_t c = 0; c < _channels; c++) {
                                        out._data[i1 + c] = _data[i0 + c];
                                }
                        }
                }
        }

        size_t Image::length()
        {
                return _width * _height * _channels;
        }
        
        size_t Image::byte_length()
        {
                return length() * sizeof(float);
        }

        Image &Image::operator=(const Image &other)
        {
                if (&other != this) {
                    init(other._type, other._width, other._height);
                    _data = other._data;
                }
                return *this;
        }
 
        std::vector<uint8_t> Image::export_byte_data()
        {
                std::vector<uint8_t> normalised_byte_data(_data.size(), 0);

                for (size_t datum = 0; datum < _data.size(); datum++) {
                    normalised_byte_data[datum] = (uint8_t) (_data[datum] * 255.0f);
                }
                return normalised_byte_data;
        }

        class IKernel
        {
        public:
                virtual ~IKernel() = default;
                virtual float apply(Image& image, size_t x, size_t y) = 0;
        };

        class Kernel3x3 : public IKernel
        {
        protected:
                float pixels_[9];

        public:
                ~Kernel3x3() override = default;
                
                float apply(Image& image, size_t x, size_t y) override {
                        float value;
                        if ((x > 0)
                            && (y > 0)
                            && (x < image.width() - 1)
                            && (y < image.height() - 1)) {
                                get_pixels_values(image, x, y);
                                return apply_to_pixels();
                        } else {
                                value = image.get(0, x, y);
                        }
                        return value;
                }
                
                void get_pixels_values(Image& image, size_t x, size_t y) {
                        pixels_[0] = image.get(0, x - 1, y - 1); 
                        pixels_[1] = image.get(0, x - 1, y); 
                        pixels_[2] = image.get(0, x - 1, y + 1); 
                        pixels_[3] = image.get(0, x,     y - 1); 
                        pixels_[4] = image.get(0, x,     y); 
                        pixels_[5] = image.get(0, x,     y + 1); 
                        pixels_[6] = image.get(0, x + 1, y - 1); 
                        pixels_[7] = image.get(0, x + 1, y); 
                        pixels_[8] = image.get(0, x + 1, y + 1);
                }
                
                virtual float apply_to_pixels() = 0;
        };

        class Dilate : public Kernel3x3
        {
        public:
                ~Dilate() override = default;
                
                float apply_to_pixels() override {
                        float value = 0.0f;
                        for (int i = 0; i < 9; i++) {
                                if (pixels_[i] > value)
                                        value = pixels_[i];
                        }
                        return value;
                }
        };

        class Erode : public Kernel3x3
        {
        public:
                ~Erode() override = default;
                
                float apply_to_pixels() override {
                        float value = 1.0f;
                        for (int i = 0; i < 9; i++) {
                                if (pixels_[i] < value)
                                        value = pixels_[i];
                        }
                        return value;
                }
        };
        
        void Image::erode_slow(size_t n, Image& out)
        {
                for (size_t i = 0; i < n; i++)
                        erode_slow(out);
        }
        
        void Image::erode_slow(Image& out)
        {
                Erode erode;
                out.init(BW, _width, _height);
                for (size_t y = 0; y < _height; y++) {
                        for (size_t x = 0; x < _width; x++) {
                                float value = erode.apply(*this, x, y);
                                out.set(0, x, y, value);
                        }
                }
        }
        
        void Image::dilate_slow(size_t n, Image& out)
        {
                for (size_t i = 0; i < n; i++)
                        dilate_slow(out);
        }
        
        void Image::dilate_slow(Image& out)
        {
                Dilate dilate;
                out.init(BW, _width, _height);
                for (size_t y = 0; y < _height; y++) {
                        for (size_t x = 0; x < _width; x++) {
                                float value = dilate.apply(*this, x, y);
                                out.set(0, x, y, value);
                        }
                }
        }

        void copy_border(float *in, float *out, size_t w, size_t h) {
                size_t index;

                index = 0;
                for (size_t x = 0; x < w; x++) {
                        out[index] = in[index];
                        index++;
                }
                
                index = (h-1) * w;
                for (size_t x = 0; x < w; x++) {
                        out[index] = in[index];
                        index++;
                }
                
                index = w;
                for (size_t y = 1; y < h-1; y++) {
                        out[index] = in[index];
                        index += w;
                }
                
                index = w + w-1;
                for (size_t y = 1; y < h-1; y++) {
                        out[index] = in[index];
                        index += w;
                }
        }

        void do_dilate(float *in, float *out, size_t w, size_t h) {
                size_t w1 = w - 1;
                size_t h1 = h - 1;
                size_t index = 0;
                float sum;
                
                for (size_t y = 1; y < h1; y++) {
                        index = y * w + 1;
                        for (size_t x = 1; x < w1; x++) {
                                sum = (in[index] + in[index+1] + in[index-1]
                                       + in[index+w] + in[index+w+1] + in[index+w-1]
                                       + in[index-w] + in[index-w+1] + in[index-w-1]);
                                if (sum > 0.0f)
                                        out[index] = 1.0f;
                                else 
                                        out[index] = 0.0f;
                                index++;
                        }
                }
                
        }
        
        void dilate(float *in, float *out, size_t w, size_t h) {
                copy_border(in, out, w, h); 
                do_dilate(in, out, w, h);
        }

        void dilate_n(size_t n, float *in, float *out, size_t w, size_t h) {
                float *a = in;
                float *b = out;

                for (size_t i = 0; i < n; i++) {
                        dilate(a, b, w, h);
                        float *tmp = a;
                        a = b;
                        b = tmp;
                }

                if (a != out) {
                        memcpy(out, a, w * h * sizeof(float));
                }
        }
        
        void Image::dilate(size_t n, Image& out) {
                if (_type != BW)
                        throw std::runtime_error("Dilate: not a BW image");

                Image copy = *this;
                out.init(BW, _width, _height);
                
                float* a = &copy._data[0];
                float* b = &out._data[0];
                dilate_n(n, a, b, _width, _height);
        }


        void do_erode(float *in, float *out, size_t w, size_t h) {
                size_t w1 = w - 1;
                size_t h1 = h - 1;
                size_t index = 0;
                float prod;
                
                for (size_t y = 1; y < h1; y++) {
                        index = y * w + 1;
                        for (size_t x = 1; x < w1; x++) {
                                prod = (in[index] * in[index+1] * in[index-1]
                                       * in[index+w] * in[index+w+1] * in[index+w-1]
                                       * in[index-w] * in[index-w+1] * in[index-w-1]);
                                if (prod == 0.0f)
                                        out[index] = 0.0f;
                                else
                                        out[index] = 1.0f;
                                index++;
                        }
                }
                
        }
        
        void erode(float *in, float *out, size_t w, size_t h) {
                copy_border(in, out, w, h); 
                do_erode(in, out, w, h);
        }

        void erode_n(size_t n, float *in, float *out, size_t w, size_t h) {
                float *a = in;
                float *b = out;

                for (size_t i = 0; i < n; i++) {
                        erode(a, b, w, h);
                        float *tmp = a;
                        a = b;
                        b = tmp;
                }

                if (a != out) {
                        memcpy(out, a, w * h * sizeof(float));
                }
        }
        
        void Image::erode(size_t n, Image& out) {
                if (_type != BW)
                        throw std::runtime_error("Erode: not a BW image");

                Image copy = *this;
                out.init(BW, _width, _height);
                
                float* a = &copy._data[0];
                float* b = &out._data[0];
                erode_n(n, a, b, _width, _height);
        }
}
