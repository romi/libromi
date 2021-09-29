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
#include <r.h>
#include "rpc/CameraAdaptor.h"
#include "rpc/MethodsCamera.h"

namespace romi {

        CameraAdaptor::CameraAdaptor(ICamera& camera)
                : camera_(camera)
        {
        }

        void CameraAdaptor::execute(const std::string& method,
                                    JsonCpp &params,
                                    rpp::MemBuffer& result,
                                    RPCError &error)
        {
                (void) params;

                error.code = 0;
                result.clear();
                
                try {
                        if (method == MethodsCamera::grab_jpeg_binary) {
                                
                                rpp::MemBuffer& jpeg = camera_.grab_jpeg();
                                result.append(jpeg); // TODO: can we avoid a copy?
                                
                        } else {
                                r_warn("Unknown method: %s", method.c_str());
                                r_debug("Known methods: %s", MethodsCamera::grab_jpeg_binary);
                                error.code = RPCError::kMethodNotFound;
                                error.message = "Unknown method";
                        }
                        
                } catch (std::exception& e) {
                        error.code = RPCError::kInternalError;
                        error.message = e.what();
                }
        }
        
        
        void CameraAdaptor::execute(const std::string& method, JsonCpp& params,
                                    JsonCpp& result, RPCError& error)
        {
                (void) params;

                error.code = 0;
                                
                try {
                        if (method == MethodsCamera::grab_jpeg_json) {
                                grab_jpeg_json(result, error);
                        } else {
                                error.code = RPCError::kMethodNotFound;
                                error.message = "Unknown method";
                        }
                        
                } catch (std::exception& e) {
                        error.code = RPCError::kInternalError;
                        error.message = e.what();
                }
        }
        
                static constexpr const char *grab_jpeg_json = "camera-grab-jpeg";
                        
        void CameraAdaptor::grab_jpeg_json(JsonCpp& result, RPCError& error)
        {
                rpp::MemBuffer& jpeg = camera_.grab_jpeg();

                if (jpeg.size() > 0) {
                        encode(jpeg, result);
                        
                } else {
                        error.code = RPCError::kInternalError;
                        error.message = "Failed to grab the image";                        
                }
        }

        void CameraAdaptor::encode(rpp::MemBuffer& jpeg, JsonCpp& result)
        {
                // TODO: that's two copies...
                char *data = encode_base64(jpeg.data().data(), jpeg.size());
                json_object_setstr(result.ptr(), "jpeg", data);
                r_free(data);
        }
        
        char *CameraAdaptor::encode_base64(const uint8_t *s, size_t ilen)
        {
                static const char table[] = {
                        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                        'w', 'x', 'y', 'z', '0', '1', '2', '3',
                        '4', '5', '6', '7', '8', '9', '+', '/'
                };

                size_t olen = 4 * ((ilen + 2) / 3);

                char *t = (char *) r_alloc(olen+1);
                t[olen] = 0;

                uint8_t a, b, c;
                uint32_t p;
        
                for (size_t i = 0, j = 0; i < ilen;) {
                        size_t n = ilen - i;
                        if (n > 3) n = 3;

                        switch (n) {
                        case 3:
                                a = (i < ilen)? s[i++] : 0;
                                b = (i < ilen)? s[i++] : 0;
                                c = (i < ilen)? s[i++] : 0;
                                p = (uint32_t) (a << 0x10) + (uint32_t) (b << 0x08) + c;
                                t[j++] = table[(p >> 18) & 0x3F];
                                t[j++] = table[(p >> 12) & 0x3F];
                                t[j++] = table[(p >> 6) & 0x3F];
                                t[j++] = table[p & 0x3F];
                                break;
                        case 2:
                                a = (i < ilen)? s[i++] : 0;
                                b = (i < ilen)? s[i++] : 0;
                                p = (uint32_t)(a << 0x10) + (uint32_t)(b << 0x08);
                                t[j++] = table[(p >> 18) & 0x3F];
                                t[j++] = table[(p >> 12) & 0x3F];
                                t[j++] = table[(p >> 6) & 0x3F];
                                t[j++] = '=';
                                break;
                        case 1:
                                a = (i < ilen)? s[i++] : 0;
                                p = (uint32_t)(a << 0x10);
                                t[j++] = table[(p >> 18) & 0x3F];
                                t[j++] = table[(p >> 12) & 0x3F];
                                t[j++] = '=';
                                t[j++] = '=';
                                break;
                        default:
                                break;
                        }
                }
                
                return t;
        }
}
