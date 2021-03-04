#include "gmock/gmock.h"
#include "IRPCHandler.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockRPCHandler : public rcom::IRPCHandler
{
public:
        MOCK_METHOD(void, execute, (const char *method, JsonCpp& params, JsonCpp& result, rcom::RPCError& error), (override));
};
#pragma GCC diagnostic pop