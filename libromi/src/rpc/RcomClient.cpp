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
#include <MessageLink.h>
#include "rpc/RcomClient.h"

namespace romi {
        
        std::unique_ptr<IRPCClient> RcomClient::create(const std::string& topic,
                                                       double timeout_seconds)
        {
                std::unique_ptr<rcom::IMessageLink> link
                        = std::make_unique<rcom::MessageLink>(topic, timeout_seconds);
                return std::make_unique<RcomClient>(link);
        }

        RcomClient::RcomClient(std::unique_ptr<rcom::IMessageLink>& link)
                : link_(), buffer_()
        {
                link_ = std::move(link);
        }

        static int32_t to_membuffer(void* userdata, const char* s, size_t len)
        {
                rpp::MemBuffer *serialised = reinterpret_cast<rpp::MemBuffer*>(userdata);
                serialised->append((uint8_t *) s, len);
                return 0;
        }

        void RcomClient::execute(const std::string& method, JsonCpp &params,
                                JsonCpp &result, RPCError &error)
        {
                r_debug("RcomClient::execute");

                try {

                        try_execute(method, params, result, error);
                        
                } catch (std::exception& e) {
                        error.code = RPCError::kInternalError;
                        error.message = e.what();                        
                }
        }
        
        void RcomClient::try_execute(const std::string& method, JsonCpp &params,
                                    JsonCpp &result, RPCError &error)
        { 
                make_request(method, params);
                if (send_request(rcom::kTextMessage, error)
                    && receive_response(buffer_, error)) {
                        parse_response(result, error);
                }
        }

        void RcomClient::make_request(const std::string& method, JsonCpp &params)
        {
                JsonCpp request = JsonCpp::construct("{\"method\": \"%s\"}",
                                                     method.c_str());
                
                // FIXME: use C++ API?
                json_object_set(request.ptr(), "params", params.ptr());
                
                // FIXME! improve JsonCpp and/or MemBuffer
                // Yet another copy
                buffer_.clear();
                json_serialise(request.ptr(), k_json_compact, to_membuffer,
                               reinterpret_cast<void*>(&buffer_));
        }

        bool RcomClient::send_request(rcom::MessageType type, RPCError &error)
        {
                bool success = false;
                if (link_->send(buffer_, type)) {
                        success = true;
                } else {
                        error.code = RPCError::kSendError;
                        error.message = "RcomClient: Sending failed";
                }
                return success;
        }

        bool RcomClient::receive_response(rpp::MemBuffer& buffer, RPCError &error)
        {
                bool success = false;
                if (link_->recv(buffer, 0.1)) {
                        success = true;
                } else {
                        set_error(error);
                }
                return success;
        }

        void RcomClient::parse_response(JsonCpp &result, RPCError &error)
        {
                try {
                        result = JsonCpp::parse(buffer_);
                                
                } catch (std::exception& e) {
                        error.code = RPCError::kParseError;
                        error.message = "RcomClient: Parsing response failed";
                }
        }
        
        void RcomClient::set_error(RPCError &error)
        {
                switch (link_->recv_status()) {
                case rcom::kRecvError:
                        error.code = RPCError::kReceiveError;
                        error.message = "RcomClient: Receive failed";
                        break;
                case rcom::kRecvClosed:
                        error.code = RPCError::kLinkClosed;
                        error.message = "RcomClient: Link closed";
                        break;
                case rcom::kRecvTimeOut:
                        error.code = RPCError::kReceiveTimeout;
                        error.message = "RcomClient: Timeout";
                        break;
                case rcom::kRecvText:
                case rcom::kRecvBinary:
                default:
                        error.code = 0;
                        break;
                }
        }

        void RcomClient::execute(const std::string& method,
                                 JsonCpp &params,
                                 rpp::MemBuffer& result,
                                 RPCError &error)
        {
                r_debug("RcomClient::execute");

                try {

                        try_execute(method, params, result, error);
                        
                } catch (std::exception& e) {
                        error.code = RPCError::kInternalError;
                        error.message = e.what();                        
                }
        }
        
        void RcomClient::try_execute(const std::string& method,
                                     JsonCpp &params,
                                     rpp::MemBuffer& result,
                                     RPCError &error)
        { 
                make_request(method, params);
                if (send_request(rcom::kBinaryMessage, error)) {
                        receive_response(result, error);
                }
        }
}
