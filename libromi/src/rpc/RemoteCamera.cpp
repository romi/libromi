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
#include "ImageIO.h"
#include "rpc/RemoteCamera.h"
#include "rpc/MethodsCamera.h"

namespace romi {
        
        RemoteCamera::RemoteCamera(std::shared_ptr<rcom::IRPCHandler>& client)
                : RemoteStub(client), output_()
        {
        }
        
        bool RemoteCamera::grab(Image &image) 
        {
                bool success = false;
                grab_jpeg();
                if (output_.size() > 0) {
                        success = ImageIO::load_jpg(image,
                                                    output_.data().data(),
                                                    output_.size());
                }
                return success;
        }
        
        rpp::MemBuffer& RemoteCamera::grab_jpeg()
        {
                JsonCpp result;
                
                r_debug("RemoteCamera::grab_jpeg");
                output_.clear();

                bool success = execute_with_result(MethodsCamera::grab_jpeg, result);
                if (success) {
                        success = decode(result);
                }

                return output_;
        }

        bool RemoteCamera::decode(JsonCpp& result)
        {
                const char *jpeg_base64 = result.str("jpeg");
                size_t length = strlen(jpeg_base64);
                return try_decode_base64(jpeg_base64, length);;
        }

        bool RemoteCamera::try_decode_base64(const char *data, size_t input_length)
        {
                bool success = false;
                
                if ((input_length % 4) == 0
                    && assert_values(data, input_length)) {
                        success = decode_base64(data, input_length);
                }
                return success;
        }
        
        bool RemoteCamera::assert_values(const char *data, size_t len)
        {
                bool success = true;

                size_t newlen = len;
                if (len > 1 && data[len - 1] == '=')
                        newlen--;
                if (len > 2 && data[len - 2] == '=')
                        newlen--;
                
                for (size_t i = 0; i < newlen; i++) {
                        if (decoding_table_[(int)data[i]] == 255) {
                                success = false;
                                break;
                        }
                }
                return success;
        }
        
        // https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
        bool RemoteCamera::decode_base64(const char *data, size_t input_length)
        {
                size_t output_length = input_length / 4 * 3;
        
                if (data[input_length - 1] == '=')
                        output_length--;
                if (data[input_length - 2] == '=')
                        output_length--;

                size_t i = 0;
                size_t j = 0;
        
                while (i < input_length) {

                        uint32_t sextet_a = get_value(data, i++);
                        uint32_t sextet_b = get_value(data, i++);
                        uint32_t sextet_c = get_value(data, i++);
                        uint32_t sextet_d = get_value(data, i++);

                        uint32_t triple = ((sextet_a << 3 * 6)
                                           | (sextet_b << 2 * 6)
                                           | (sextet_c << 1 * 6)
                                           | (sextet_d << 0 * 6));
                
                        if (j++ < output_length)
                                output_.put((uint8_t) ((triple >> 2 * 8) & 0xff));
                        if (j++ < output_length)
                                output_.put((uint8_t) ((triple >> 1 * 8) & 0xff));
                        if (j++ < output_length)
                                output_.put((uint8_t) ((triple >> 0 * 8) & 0xff));
                }

                return true;
        }
        
        uint8_t RemoteCamera::get_value(const char *data, size_t i)
        {
                uint8_t value;
                if (data[i] == '=') {
                        value = 0;
                } else {
                        value = decoding_table_[(int)data[i]];
                }
                return value;
        }

        // https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
        void RemoteCamera::build_decoding_table()
        {
                char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                         'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                         'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                         'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                         'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                         'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                         'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                         '4', '5', '6', '7', '8', '9', '+', '/'};
                
                for (int i = 0; i < 256; i++)
                        decoding_table_[i] = 255;
                
                for (uint8_t i = 0; i < 64; i++)
                        decoding_table_[(int)encoding_table[i]] = (uint8_t) i;
        }
}
