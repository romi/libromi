#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "rpc/RcomMessageHandler.h"
#include "WebSocket.mock.h"
#include "../mock/mock_rpchandler.h"

using namespace std;
using namespace testing;
using namespace romi;

class rcommessagehandler_tests : public ::testing::Test
{
protected:
        MockRPCHandler handler_;
        MockWebSocket websocket_;
        std::string output_;
        RPCError return_error_;
        std::string sent_method_;
        JsonCpp sent_params_;
        JsonCpp result_;
        
	rcommessagehandler_tests()
                : handler_(),
                  websocket_(),
                  output_(),
                  return_error_(),
                  sent_method_(),
                  sent_params_(),
                  result_() {
        }

	~rcommessagehandler_tests() override = default;

	void SetUp() override {
                output_ = "";
                return_error_.code = 0;
                return_error_.message = "";
                sent_method_ = "";
                sent_params_ = json_null();
                result_ = json_null();
        }

	void TearDown() override {}

public:
        
        bool send(rpp::MemBuffer& message, rcom::MessageType type) {
                (void) type;
                output_ = message.tostring();
                r_err("send: %s", output_.c_str());
                return true;
        }

        int error_code() {
                JsonCpp reply = JsonCpp::parse(output_.c_str());
                return (int) reply.get("error").num("code");
        }

        JsonCpp get_result() {
                JsonCpp reply = JsonCpp::parse(output_.c_str());
                return reply.get("result");
        }

        void arrange_error(int16_t code, const char *message) {
                return_error_.code = code;
                return_error_.message = message;
        }

        void set_error(const std::string& method,
                       JsonCpp& params,
                       JsonCpp& result,
                       RPCError& error) {
                (void) result;
                r_debug("method=%s", method.c_str());
                sent_method_ = method;
                sent_params_ = params;
                error.code = return_error_.code;
                error.message = return_error_.message;
        }
        
        void set_result(const std::string& method,
                        JsonCpp& params,
                        JsonCpp& result,
                        RPCError& error) {
                r_debug("method=%s", method.c_str());
                sent_method_ = method;
                sent_params_ = params;
                error.code = 0;
                result = result_;
        }
};

TEST_F(rcommessagehandler_tests, request_with_invalid_json_returns_appropriate_error_code)
{
        // Arrange
        RcomMessageHandler message_handler(handler_);
        rpp::MemBuffer message;
        message.printf("this is invalid json");

        EXPECT_CALL(websocket_, send(_,_))
                .WillOnce(DoAll(Invoke(this, &rcommessagehandler_tests::send),
                                Return(true)));
        
        // Act
        message_handler.onmessage(websocket_, message);

        // Assert
        ASSERT_EQ(error_code(), RPCError::kParseError);
}

TEST_F(rcommessagehandler_tests, request_with_missing_method_returns_appropriate_error_code)
{
        // Arrange
        RcomMessageHandler message_handler(handler_);
        rpp::MemBuffer message;
        message.printf("{}");

        EXPECT_CALL(websocket_, send(_,_))
                .WillOnce(DoAll(Invoke(this, &rcommessagehandler_tests::send),
                                Return(true)));
        
        // Act
        message_handler.onmessage(websocket_, message);

        // Assert
        ASSERT_EQ(error_code(), RPCError::kInvalidRequest);
}

TEST_F(rcommessagehandler_tests, request_with_unknown_method_returns_appropriate_error_code)
{
        // Arrange
        RcomMessageHandler message_handler(handler_);
        rpp::MemBuffer message;
        message.printf("{\"method\": \"toto\"}");
        arrange_error(RPCError::kMethodNotFound, "MESSAGE");

        EXPECT_CALL(handler_, execute(_,_,An<JsonCpp&>(),_))
                .WillOnce(Invoke(this, &rcommessagehandler_tests::set_error));
        
        EXPECT_CALL(websocket_, send(_,_))
                .WillOnce(DoAll(Invoke(this, &rcommessagehandler_tests::send),
                                Return(true)));
        
        // Act
        message_handler.onmessage(websocket_, message);

        // Assert
        ASSERT_EQ(error_code(), RPCError::kMethodNotFound);
}

TEST_F(rcommessagehandler_tests, response_contains_expected_result)
{
        // Arrange
        RcomMessageHandler message_handler(handler_);
        rpp::MemBuffer message;
        message.printf("{\"method\": \"toto\"}");
        result_ = JsonCpp::parse("{\"key\": \"test value\"}");

        EXPECT_CALL(handler_, execute(_,_,An<JsonCpp&>(),_))
                .WillOnce(Invoke(this, &rcommessagehandler_tests::set_result));
        
        EXPECT_CALL(websocket_, send(_,_))
                .WillOnce(DoAll(Invoke(this, &rcommessagehandler_tests::send),
                                Return(true)));
        
        // Act
        message_handler.onmessage(websocket_, message);

        // Assert
        JsonCpp received_result = get_result();
        ASSERT_STREQ(received_result.str("key"), "test value");
}

