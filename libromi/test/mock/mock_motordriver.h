#include "gmock/gmock.h"
#include "api/IMotorDriver.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockMotorDriver : public romi::IMotorDriver
{
public:
        MOCK_METHOD(bool, moveat, (double left, double right), (override));
        MOCK_METHOD(bool, stop, (), (override));
        MOCK_METHOD(bool, get_encoder_values, (double &left, double &right, double &timestamp), (override));
};
#pragma GCC diagnostic pop
