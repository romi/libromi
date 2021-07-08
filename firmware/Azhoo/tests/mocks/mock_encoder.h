#include "gmock/gmock.h"
#include "IEncoder.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockEncoder : public IEncoder
{
public:
        MOCK_METHOD(int32_t, get_position, (), (override));
        MOCK_METHOD(int32_t, positions_per_revolution, (), (override));
};
#pragma GCC diagnostic pop
