#include "gmock/gmock.h"
#include "rpc/IRPCHandler.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockRPCHandler : public romi::IRPCHandler
{
public:
        MOCK_METHOD(void, execute, (const std::string& method, JsonCpp& params, JsonCpp& result, romi::RPCError& error), (override));
        MOCK_METHOD(void, execute, (const std::string& method, JsonCpp& params, rpp::MemBuffer& result, romi::RPCError& error), (override));
};
#pragma GCC diagnostic pop
