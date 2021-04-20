#include "gmock/gmock.h"
#include "rpc/IRPCClient.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockRPCClient : public romi::IRPCClient
{
public:
        MOCK_METHOD(void, execute, (const std::string& method, JsonCpp& params, JsonCpp& result, romi::RPCError& error), (override));
};
#pragma GCC diagnostic pop
